#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LEN 100

int main(int argc, char **argv) {
  int rank, size;

  // Inisialisasi MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  if (size != 6) {
    if (rank == 0) {
      printf("ERROR: Program ini membutuhkan tepat 6 proses (1 Master + 5 "
             "Worker).\n");
      printf("Gunakan perintah: mpirun -np 6 ./analisis_nilai\n");
    }
    MPI_Finalize();
    return 1;
  }

  int total_data = MAX_LINES;
  float global_data[MAX_LINES];

  // ==========================================
  // Anggota 1: Master (Rank 0) membaca file CSV
  // ==========================================
  if (rank == 0) {
    FILE *fp = fopen("data_nilai.csv", "r");
    if (!fp) {
      printf("ERROR: Gagal membuka file data_nilai.csv\n");
      MPI_Abort(MPI_COMM_WORLD, 1);
    }

    char line[MAX_LINE_LEN];
    // Skip baris pertama (header)
    fgets(line, MAX_LINE_LEN, fp);

    int i = 0;
    // Baca data baris per baris
    while (fgets(line, MAX_LINE_LEN, fp) && i < MAX_LINES) {
      // Format: NIM,Nilai
      char *token = strtok(line, ",");
      token = strtok(NULL, ","); // Ambil token kedua (Nilai)
      if (token) {
        global_data[i] = atof(token);
        i++;
      }
    }
    fclose(fp);
  }

  // ==========================================
  // Anggota 2: Distribusi Data dengan MPI_Scatterv
  // ==========================================
  int sendcounts[6];
  int displs[6];

  // Kalkulasi pembagian data (100 data / 6 node)
  int remainder = total_data % size;
  int offset = 0;

  for (int i = 0; i < size; i++) {
    sendcounts[i] = total_data / size; // Bagian dasar (16)
    if (i < remainder) {
      sendcounts[i]++; // Sisa data dibagikan ke rank awal
    }
    displs[i] = offset;
    offset += sendcounts[i];
  }

  // Alokasi memori lokal untuk setiap node
  int local_count = sendcounts[rank];
  float *local_data = (float *)malloc(local_count * sizeof(float));

  // Scatter data dari root ke semua node
  MPI_Scatterv(global_data, sendcounts, displs, MPI_FLOAT, local_data,
               local_count, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // ==========================================
  // Anggota 3: Komputasi Worker (Lokal)
  // ==========================================
  float local_max = -1.0f;
  float local_min = 200.0f; // inisialisasi awal > max mungkin
  float local_sum = 0.0f;

  for (int i = 0; i < local_count; i++) {
    float val = local_data[i];
    if (val > local_max)
      local_max = val;
    if (val < local_min)
      local_min = val;
    local_sum += val;
  }

  // ==========================================
  // Anggota 4: MPI_Reduce (Agregasi Global)
  // ==========================================
  float global_max, global_min, global_sum;

  MPI_Reduce(&local_max, &global_max, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&local_min, &global_min, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  // ==========================================
  // Anggota 5: Tracking Detail Pekerjaan (MPI_Gather)
  // ==========================================
  int gathered_counts[6];
  float gathered_maxs[6];
  float gathered_mins[6];
  char gathered_names[6][MPI_MAX_PROCESSOR_NAME];

  // Root mengumpulkan metrik dari semua rank
  MPI_Gather(processor_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR,
             gathered_names, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0,
             MPI_COMM_WORLD);
  MPI_Gather(&local_count, 1, MPI_INT, gathered_counts, 1, MPI_INT, 0,
             MPI_COMM_WORLD);
  MPI_Gather(&local_max, 1, MPI_FLOAT, gathered_maxs, 1, MPI_FLOAT, 0,
             MPI_COMM_WORLD);
  MPI_Gather(&local_min, 1, MPI_FLOAT, gathered_mins, 1, MPI_FLOAT, 0,
             MPI_COMM_WORLD);

  // ==========================================
  // Anggota 6: Output / Cetak Hasil Akhir di Master
  // ==========================================
  if (rank == 0) {
    float global_avg = global_sum / total_data;

    printf("\n");
    printf("=================================================================================\n");
    printf("                     HASIL ANALISIS NILAI MAHASISWA (MPI)                        \n");
    printf("=================================================================================\n");
    printf(" Total Data Diproses : %d baris\n", total_data);
    printf(" Nilai Tertinggi     : %.2f\n", global_max);
    printf(" Nilai Terendah      : %.2f\n", global_min);
    printf(" Rata-rata Kelas     : %.2f\n", global_avg);
    printf("=================================================================================\n");
    printf("\n");
    
    // Cetak Tabel
    printf("+------+----------+----------------------+-----------+---------+---------+\n");
    printf("| Rank | Role     | Hostname             | Jml Data  | Max Loc | Min Loc |\n");
    printf("+------+----------+----------------------+-----------+---------+---------+\n");
    
    for (int i = 0; i < size; i++) {
      char role[20];
      if (i == 0) {
        strcpy(role, "Master");
      } else {
        sprintf(role, "Worker %d", i);
      }
      printf("| %-4d | %-8s | %-20.20s | %-9d | %-7.2f | %-7.2f |\n", 
             i, role, gathered_names[i], gathered_counts[i], gathered_maxs[i], gathered_mins[i]);
    }
    printf("+------+----------+----------------------+-----------+---------+---------+\n");
    
    // Penjabaran Flow
    printf("\n");
    printf("ALUR PENGERJAAN MPI (WORKFLOW):\n");
    printf("1. [Master]   %s (Rank 0) membaca file data_nilai.csv berisi %d baris data.\n", gathered_names[0], total_data);
    printf("2. [Master]   Memecah dan mendistribusikan potongan data menggunakan MPI_Scatterv ke %d node.\n", size);
    
    printf("3. [Workers]  ");
    for(int i=1; i<size; i++) {
        printf("%s", gathered_names[i]);
        if(i < size-1) printf(", ");
    }
    printf(" menerima potongan datanya.\n");
    
    printf("4. [Semua]    Melakukan komputasi pencarian nilai Max, Min, dan Sum secara PARALEL di masing-masing mesin.\n");
    printf("5. [Master]   Mengumpulkan hasil perhitungan parsial dari semua Worker menggunakan MPI_Reduce dan MPI_Gather.\n");
    printf("6. [Master]   Menghitung rata-rata kelas dan menyatukan laporan menjadi tabel di atas.\n");
    printf("=================================================================================\n\n");
  }

  // Membersihkan memori lokal
  free(local_data);
  MPI_Finalize();

  return 0;
}

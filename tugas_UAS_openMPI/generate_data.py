import csv
import random

def generate_csv():
    filename = 'data_nilai.csv'
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['NIM', 'Nilai'])
        for i in range(1, 101):
            nim = f"2410605{i:04d}"
            nilai = round(random.uniform(40.0, 100.0), 2)
            writer.writerow([nim, nilai])

if __name__ == '__main__':
    generate_csv()

#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

struct mhs {
    int nim;
    char nama[50];
    mhs *kiri;
    mhs *kanan;

    mhs(int ni, const char na[50]) {
        nim = ni;
        strcpy(nama, na);
        kiri = kanan = nullptr;
    }
};

void tambahdata(mhs *&akar, int nim, const char nama[50]) {
    if (akar == nullptr) {
        akar = new mhs(nim, nama);
        return;
    }

    mhs *tolong = akar;

    while (true) {
        if (nim < tolong->nim) {
            if (tolong->kiri == nullptr) {
                tolong->kiri = new mhs(nim, nama);
                return;
            }
            tolong = tolong->kiri;
        } else if (nim > tolong->nim) {
            if (tolong->kanan == nullptr) {
                tolong->kanan = new mhs(nim, nama);
                return;
            }
            tolong = tolong->kanan;
        } else {
            cout << "NIM " << nim << " sudah ada." << endl;
            return;
        }
    }
}

void caridata(mhs *akar, int ygdicari) {
    mhs *tolong = akar;

    while (tolong != nullptr) {
        if (ygdicari == tolong ->nim) {
            cout << tolong->nim << " - " << tolong->nama << endl;
            return;
        }
        
        if (ygdicari < tolong->nim) {
            tolong = tolong->kiri;
        } else {
            tolong = tolong->kanan;
        }
    }

    cout << "NIM " << ygdicari << " blom terdata" << endl;
}

void hapusdata(mhs *&akar, int ygdihapus) {
    if (akar == nullptr) {
        cout << "Tidak ada data dengan NIM " << ygdihapus << endl;
        return;
    }

    if (ygdihapus < akar->nim) {
        hapusdata(akar->kiri , ygdihapus);
    } else if (ygdihapus > akar->nim) {
        hapusdata(akar->kanan , ygdihapus);
    } else {
        mhs *tolong = akar;

        if (akar->kiri == nullptr) {
            akar = akar->kanan;
        } else if (akar->kanan == nullptr) {
            akar = akar->kiri;
        } else {
            mhs *ganti = akar->kanan;
            while (ganti->kiri != nullptr) {
                ganti = ganti->kiri;
            }
            akar->nim = ganti->nim;
            strcpy(akar->nama, ganti->nama);
            hapusdata(akar->kanan , ganti->nim);
            return;
        }
        delete tolong;
    }
}

void tampildata(mhs *akar) {
    if (akar == nullptr) {
        return;
    } else {
        tampildata(akar->kiri);
        cout << akar->nim << " - " << akar->nama << endl;
        tampildata(akar->kanan);
    }
}

void simpanfile(mhs *akar, FILE *file) {
    if (akar == nullptr) return;

    simpanfile(akar->kiri, file);
    fwrite(&akar->nim, sizeof(int), 1, file);
    fwrite(akar->nama, sizeof(char), 50, file);
    simpanfile(akar->kanan, file); 
}

void savedata(mhs *akar) {
    FILE *file = fopen("datamhs.txt", "wb");
    if (file == nullptr) {
        cout << "Gagal membuka file untuk menyimpan." << endl;
        return;
    }
    simpanfile(akar, file);
    fclose(file);
}

void loaddata(mhs *&akar) {
    FILE *file = fopen("datamhs.txt", "rb");
    if (file == nullptr) {
        cout << "File tidak ditemukan atau gagal dibuka." << endl;
        return;
    }

    int nim;
    char nama[50];

    while (fread(&nim, sizeof(int), 1, file) == 1) {
        if (fread(nama, sizeof(char), 50, file) != 50) {
            cout << "Terjadi kesalahan saat membaca nama." << endl;
            break;
        }
        tambahdata(akar, nim, nama);
    }

    fclose(file);
    cout << "Data berhasil dimuat dari file." << endl;
}

int main() {
    mhs *akar = nullptr;
    loaddata(akar);
    int pilih, ygdicari, ygdihapus, nim;
    char nama[50];

    do {
        system("cls");
        cout << "Menu:" << endl;
        cout << "1. Tambah data" << endl;
        cout << "2. Cari data" << endl;
        cout << "3. Hapus data" << endl;
        cout << "4. Tampilkan data" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih)
        {
        case 1:
            cout << "Masukkan NIM: ";
            cin >> nim;

            cin.ignore();
            cout << "Masukkan Nama: ";
            cin.getline(nama, 50);

            tambahdata(akar, nim, nama);

            {
                FILE *file = fopen("datamhs.txt", "ab");
                if (file == nullptr) {
                    cout << "Gagal membuka file" << endl;
                    break;
                }
                fwrite(&nim, sizeof(int), 1, file);
                fwrite(nama, sizeof(char), 50, file);
                fclose(file);
            }

            cout << "Data berhasil ditambahkan" << endl;
            system("pause");
            break;

        case 2:
            cout << "Masukkan NIM yang dicari: ";
            cin >> ygdicari;
            caridata(akar, ygdicari);
            system("pause");
            break;

        case 3:
            cout << "Masukkan NIM yang akan dihapus: ";
            cin >> ygdihapus;
            hapusdata(akar, ygdihapus);

            savedata(akar);
            cout << "data berhasil diperbarui" << endl;
            system("pause");
            break;

        case 4:
            if (akar == nullptr) {
                cout << "Blom ada data" << endl;
            } else {
                cout << "Data mahasiswa:" << endl;
                tampildata(akar);
            }
            system("pause");
            break;

        case 0:
            cout << "Keluar program" << endl;
            break;

        default:
            cout << "Pilihan tidak valid" << endl;
            break;
        }

    } while (pilih != 0);
}
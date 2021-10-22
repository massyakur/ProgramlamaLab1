#include <stdio.h>
#include <stdlib.h>
int count = 0;

typedef struct Kayit
{
    int ogrNo;
    int dersKodu;
    int puan;
    int offset;
}ogrenci;
void counter()
{
    FILE *file;
    file = fopen("kayitlar.bin", "rb");
    /*
    while(fread(&s1, sizeof(ogrenci), 1, file)){
        count += sizeof(s1);
    }
    count = count/sizeof(ogrenci);
    */
    printf("\n-------------------------\n");
    fseek(file, 0, SEEK_END);
    count = ftell(file)/sizeof(ogrenci);
    printf("\n%d Kayit bulunmaktadir.\n", count);
    fclose(file);
}

void indexDosyasiOlustur()
{
    ogrenci *s, s1;
    FILE *file, *file2;
    int i=0, j;
    file = fopen("kayitlar.bin", "rb");
    file2 = fopen("index.txt", "w");
    s = (ogrenci*)calloc(count, sizeof(ogrenci));
    while(fread(&s[i], sizeof(ogrenci), 1, file))
    {
        s[i].offset = ftell(file)-sizeof(ogrenci);
        i++;
    }
    fclose(file);

    for(i=0; i<count; i++)
    {
        for(j=i+1; j<count; j++)
        {
            if(s[i].ogrNo > s[j].ogrNo)
            {
                s1 = s[i];
                s[i] = s[j];
                s[j] = s1;
            }
        }
    }
    file2 = fopen("index.txt", "w");
    for(i=0; i<count; i++)
    {
        fprintf(file2, "%d\t%d\n", s[i].ogrNo, s[i].offset);
    }
    fclose(file2);
    printf("\nindex.txt BASARIYLA OLUSTURULMUSTUR!\n");

    /*int i, j, temp, temp2;
    diziYap();
    file2 = fopen("index.txt", "r");
    for(i = 0; i<count; i++)
    {
        fscanf(file, "%d %d", &ogrNo[i], &offset[i]);
    }
    fclose(file2);
    for(i = 0; i<count; i++)
    {
        for(j = i+1; j<count; j++)
        {
            if(ogrNo[i] > ogrNo[j])
            {
                temp = ogrNo[i];
                temp2 = offset[i];
                ogrNo[i] = ogrNo[j];
                offset[i] = offset[j];
                ogrNo[j] = temp;
                offset[j] = temp2;
            }
        }
    }
    file2 = fopen("index.txt", "w");
    for(i = 0; i < count; i++)
    {
        fprintf(file2, "%d\t%d\n", ogrNo[i], offset[i]);
    }
    */
}
void kayitOlustur()
{
    ogrenci *s;
    FILE *file;
    int n, i;
    printf("Kac tane ogrenci gireceksiniz : ");
    scanf("%d", &n);
    printf("\n");
    s = (ogrenci*)calloc(n, sizeof(ogrenci));
    file = fopen("kayitlar.bin", "wb");
    if(file == NULL)
    {
      printf("Error!");
      exit(1);
    }

    for(i=0;i<n;i++){
        printf("Ogrenci numarayi giriniz : ");
        scanf("%d", &s[i].ogrNo);
        //fflush(stdin);
        printf("Ders kodunu giriniz : ");
        scanf("%d", &s[i].dersKodu);
        printf("Puan giriniz : ");
        scanf("%d", &s[i].puan);
        printf("\n");
        fwrite(&s[i], sizeof(ogrenci), 1, file);
    }
    fclose(file);
    counter();
    indexDosyasiOlustur();
}

void kayitEkle()
{
    ogrenci *s;
    FILE *file;
    int n, i;
    printf("Kac tane ogrenci gireceksiniz : ");
    scanf("%d", &n);
    printf("\n");
    s = (ogrenci*)calloc(n, sizeof(ogrenci));
    file = fopen("kayitlar.bin", "ab");
    if(file == NULL)
    {
      printf("Error!");
      exit(1);
    }

    for(i=0;i<n;i++){
        printf("Ogrenci numarayi giriniz : ");
        scanf("%d", &s[i].ogrNo);
        //fflush(stdin);
        printf("Ders kodunu giriniz : ");
        scanf("%d", &s[i].dersKodu);
        printf("Puan giriniz : ");
        scanf("%d", &s[i].puan);
        printf("\n");

        fwrite(&s[i], sizeof(ogrenci), 1, file);
    }
    fclose(file);
    counter();
    indexDosyasiOlustur();
}

void kayitBul()
{
    //diziYap();
    ogrenci s1;
    int *ogrNo, *offset;
    int search;
    printf("ARAMAK ICIN OGRENCI NO YAZINIZ : ");
    scanf("%d", &search);

    FILE *file, *file2;

    printf("\n======================================\n");
    printf("| Ogrenci No\tDers Kodu\tNotu |");
    printf("\n--------------------------------------\n");
    ogrNo = (int*)calloc(count, sizeof(int));
    offset = (int*)calloc(count, sizeof(int));
    file = fopen("index.txt", "r");
    for(int i = 0; i < count; i++)
        {
            fscanf(file, "%d %d", &ogrNo[i], &offset[i]);
        }
    fclose(file);
    file2 = fopen("kayitlar.bin", "rb");
    int low = 0, high = count-1, mid = (low+high)/2;
    while(low<=high)
    {
        if(ogrNo[mid]<search)
            low = mid+1;
        else if(ogrNo[mid]==search)
        {
            //Binary search ile ilk bulunan veri
            fseek(file2, offset[mid], SEEK_SET);
            fread(&s1, sizeof(ogrenci), 1, file2);
            printf("| %*d %*d %*d  |\n", 7, s1.ogrNo, 12, s1.dersKodu, 12, s1.puan);

            for(int i=0; i<=high; i++) //Eger ayni ogrNo birden fazla varsa asagida yazilacaktir
            {
                if(search == ogrNo[i] && mid != i)
                {

                        fseek(file2, offset[i], SEEK_SET);
                        fread(&s1, sizeof(ogrenci), 1, file2);
                        printf("| %*d %*d %*d  |\n", 7, s1.ogrNo, 12, s1.dersKodu, 12, s1.puan);

                }
            }
            break;
        }
        else
        high = mid-1;
        mid=(low+high)/2;
    }
    if(low>high)
    {
        printf("|\t\t\t\t     |\n");
        printf("|%*d NOLU OGRENCI BULUNAMADI!   |\n", 8, search);
        printf("|\t\t\t\t     |\n");
    }
    printf("======================================\n");
    fclose(file2);
}

void kayitSil()
{
    ogrenci s1, *s;
    int search, n=0, len=0, j=0;
    printf("SILMEK ICIN OGRENCI NO YAZINIZ : ");
    scanf("%d", &search);
    int *ogrNo, *offset;
    FILE *file, *file2, *file3;
    file = fopen("index.txt", "r");
    s = (ogrenci*)calloc(count-1, sizeof(ogrenci));
    ogrNo = (int*)calloc(count, sizeof(int));
    offset = (int*)calloc(count, sizeof(int));
    for(int i = 0; i < count; i++)
        {
            fscanf(file, "%d %d", &ogrNo[i], &offset[i]);
        }
    fclose(file);
    len = sizeof(*offset)/sizeof(int)*count;

    file2 = fopen("kayitlar.bin", "rb");
    int low = 0, high = count-1, mid = (low+high)/2;
    while(low<=high)
    {
        if(ogrNo[mid]<search)
            low = mid+1;
        else if(ogrNo[mid]==search)
        {
            for(int i=mid; i>0; i--) //for bottom
            {
                fseek(file2, offset[mid-i], SEEK_CUR);
                fread(&s1, sizeof(ogrenci), 1, file2);
                s[j] = s1;
                //s[j].ogrNo = s1.ogrNo;
                //s[j].dersKodu = s1.dersKodu;
                //s[j].puan = s1.puan;
                s[j].offset = offset[mid-i];
                j++;
                rewind(file2);
            }
            for(int i=mid; i<len-1; i++) //for up
            {
                fseek(file2, offset[i+1], SEEK_CUR);
                fread(&s1, sizeof(ogrenci), 1, file2);
                s[j] = s1;
                //s[j].ogrNo = s1.ogrNo;
                //s[j].dersKodu = s1.dersKodu;
                //s[j].puan = s1.puan;
                s[j].offset = offset[i+1];
                j++;
                rewind(file2);
            }
            fclose(file2);
            for(int i=0; i<count-1; i++)
            {
                for(j=i+1; j<count-1; j++)
                {
                    if(s[i].offset > s[j].offset)
                    {
                        s1 = s[i];
                        s[i] = s[j];
                        s[j] = s1;
                    }
                }
            }
            file2 = fopen("kayitlar.bin", "wb");
            for(int i=0; i<count-1; i++)
            {
                fwrite(&s[i], sizeof(ogrenci), 1, file2);
            }
            fclose(file2);
            break;
        }
        else
        high = mid-1;
        mid=(low+high)/2;
    }
    if(low>high)
    {
        printf("|\t\t\t\t     |\n");
        printf("|%*d NOLU OGRENCI BULUNAMADI!   |\n", 8, search);
        printf("|\t\t\t\t     |\n");
    }

    counter();
    indexDosyasiOlustur();
}
void kayitGuncelle()
{
    ogrenci s1, s;
    int *ogrNo, *offset, search;

    printf("PUAN DEGISTIRMEK ICIN OGRENCI NO YAZINIZ : ");
    scanf("%d", &search);

    FILE *file, *file2;

    ogrNo = (int*)calloc(count, sizeof(int));
    offset = (int*)calloc(count, sizeof(int));

    file = fopen("index.txt", "r");
    for(int i = 0; i < count; i++)
        {
            fscanf(file, "%d %d", &ogrNo[i], &offset[i]);
        }
    fclose(file);
    file2 = fopen("kayitlar.bin", "rb+");
    int low = 0, high = count-1, mid = (low+high)/2;
    while(low<=high)
    {
        if(ogrNo[mid]<search)
            low = mid+1;
        else if(ogrNo[mid]==search)
        {
            //Binary search ile ilk bulunan veri
            fseek(file2, offset[mid], SEEK_SET);
            fread(&s1, sizeof(ogrenci), 1, file2);
            s = s1;
            printf("Puani giriniz: ");
            scanf("%d", &s.puan);
            fseek(file2, offset[mid], SEEK_SET);
            fwrite(&s, sizeof(ogrenci), 1, file2);

            for(int i=0; i<=high; i++) //Eger ayni ogrNo birden fazla varsa asagida yazilacaktir
            {
                if(search == ogrNo[i] && mid != i)
                {

                    fseek(file2, offset[i], SEEK_SET);
                    fread(&s1, sizeof(ogrenci), 1, file2);
                    s = s1;
                    printf("Puani giriniz: ");
                    scanf("%d", &s.puan);
                    fseek(file2, offset[i], SEEK_SET);
                    fwrite(&s, sizeof(ogrenci), 1, file2);

                }
            }
            break;
        }
        else
        high = mid-1;
        mid=(low+high)/2;
    }
    if(low>high)
    {
        printf("|\t\t\t\t     |\n");
        printf("|%*d NOLU OGRENCI BULUNAMADI!   |\n", 8, search);
        printf("|\t\t\t\t     |\n");
    }
    printf("======================================\n");
    fclose(file2);

}

void veriDosyasiniGoster()
{
    ogrenci s1;
    FILE *file;
    file = fopen("kayitlar.bin", "rb");
    printf("\n--------------------------------------\n");
    printf("| Ogrenci No\tDers Kodu\tNotu |");
    printf("\n--------------------------------------\n");
    while(fread(&s1, sizeof(ogrenci), 1, file)){
        printf("| %*d %*d %*d  |\n", 7, s1.ogrNo, 12, s1.dersKodu, 12, s1.puan);
    }
    printf("--------------------------------------\n");
    printf("\n");
    fclose(file);
}

void indeksDosyasiniGoster()
{
    int ogrNo, offset;
    FILE *file;
    file = fopen("index.txt", "r");
    if(file==NULL)
        printf("\nDOSYA BULUNAMADI!\n");
    else
    {
        printf("\n------------------------\n");
        printf("| Ogrenci No\tOffset |");
        printf("\n------------------------\n");
        for(int i = 0; i < count; i++)
        {
            fscanf(file, "%d %d", &ogrNo, &offset);
            printf("| %*d%*d   |\n", 7, ogrNo, 11, offset);
        }
        printf("------------------------\n");
    }
    fclose(file);
    /*
    FILE *file2;
    file2 = fopen("kayitlar.bin", "rb");
    fseek(file2, offset, SEEK_SET);
    fread(&s1, sizeof(ogrenci), 1, file2);
    printf("\n%d %d %d\n", s1.ogrNo, s1.dersKodu, s1.puan);
    fclose(file2);*/
}
void indeksDosyasiniSil()
{
    int del = remove("index.txt");
    if (!del)
        printf("\nindex.txt BASARIYLA SILINMISTIR!\n");
    else
        printf("\nDOSYA BULUNAMADI/SILINEMEDI!\n");
}


int main()
{
    int sec;
    counter();
    do{
        printf("\n-------------------------\n");
        printf("1. KAYIT OLUSTUR\n");
        printf("2. EKLE\n");
        printf("3. BUL\n");
        printf("4. SIL\n");
        printf("5. GUNCELLE\n");
        printf("6. VERI DOSYASI GOSTER\n");
        printf("7. INDEX DOSYASI GOSTER\n");
        printf("8. INDEX DOSYASI SIL\n");
        printf("9. INDEX DOSYASI OLUSTUR\n");
        printf("0. CIK\n");
        printf("-------------------------\n\n");

        printf("Istediginiz numarayi giriniz : ");
        scanf("%d", &sec);

        switch(sec){
            case 1:
                kayitOlustur();
            break;
            case 2:
                kayitEkle();
            break;
            case 3:
                kayitBul();
            break;
            case 4:
                kayitSil();
            break;
            case 5:
                kayitGuncelle();
            break;
            case 6:
                veriDosyasiniGoster();
            break;
            case 7:
                indeksDosyasiniGoster();
            break;
            case 8:
                indeksDosyasiniSil();
            break;
            case 9:
                indexDosyasiOlustur();
            break;
            default:
            break;

        }
    }while(sec!=0);


    return 0;
}


/*void diziYap()
{
    FILE *file;
    int i;
    ogrNo = (int*)calloc(count, sizeof(int));
    offset = (int*)calloc(count, sizeof(int));
    file = fopen("index.txt", "r");
    for(i = 0; i<count; i++)
    {
        fscanf(file, "%d %d", &ogrNo[i], &offset[i]);
    }
    fclose(file);
}*/

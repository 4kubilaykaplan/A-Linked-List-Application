#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
enum boolean
{
    false = 0,
    true = 1
};

struct sehir
{
    struct sehir *next;
    struct sehir *prev;
    int plaka;
    char adi[50];
    char bolge[5];
    int komsuSayisi;
    char veri[1000];
    struct komsu *komsu;
};

struct komsu
{
    struct komsu *next;
    int plaka;
    char adi[50];
    char bolge[5];
};

typedef enum boolean bool;
typedef struct sehir sehir;
typedef struct komsu komsu;
FILE *fp;
int sehirSayisi=0;

sehir *dosyayiOku();
char *boslukYap(char*s, char cop);
int komsuSayisiniBul(char*s);
void dosyayaYazdir(sehir *r);
void bolgeyleListele(sehir *r);
void komsuSayisiylaListele(sehir *r);
int sayiMi (char str[]);

komsu *komsuSil(sehir *r, int x);
sehir *sehirSil(sehir *r, int x);
sehir *silMenu(sehir *r);
sehir *yeniSehirEkle(sehir *r);
sehir *sehirEkle(sehir *r, sehir *dinamik);
komsu *komsuEkle (sehir * r, sehir *dinamik);
sehir * sehirAra(sehir *r);

int main()
{
    sehir * sehirler=dosyayiOku();
    sehir * degisken=sehirler;
    int secim;
    do
    {
        printf("1-Sehir ekle\n");
        printf("2-Sehir ara\n");
        printf("3-Sehir sil\n");
        printf("4-Bolgeye gore listele\n");
        printf("5-Komsu sayisina gore listele\n");
        printf("6-Cikti olustur ve cik\n");
        printf("Seciminizi Giriniz:");
        scanf("%d", &secim);
        printf("\n");
        switch (secim)
        {
        case 1:
            degisken=yeniSehirEkle(degisken);
            break;
        case 2:
            degisken=sehirAra(degisken);
            break;
        case 3:
            degisken=silMenu(degisken);
            break;
        case 4:
            bolgeyleListele(degisken);
            break;
        case 5:
            komsuSayisiylaListele(degisken);
            break;
        case 6:
            dosyayaYazdir(degisken);
            exit(1);
        default:
            break;
        }
    }
    while (1);
    return 0;
}

sehir* dosyayiOku()
{
    fp=fopen("sehirler.txt","r");
    if (fp == NULL)
    {
        printf("\n Dosya acilamadi\n");
        fclose(fp);
    }
    else
    {
        char line[2000];
        sehir * root;
        root = NULL;
        sehir *dinamik = (sehir*)malloc(sizeof(sehir));
        char *satir;
        int komsu;

        while (fgets(line, sizeof (line), fp))
        {
            satir=boslukYap(line, ',');
            komsu=komsuSayisiniBul(satir);
            dinamik ->komsuSayisi=komsu;
            sscanf(satir, "%d %s %s %[^\n]", &dinamik ->plaka, &dinamik ->adi, &dinamik ->bolge, &dinamik->veri);
            root = sehirEkle(root, dinamik);
            sehirSayisi++;
        }
        sehir * root2=root;
        sehir * root3=root;
        sehir * root4=root;
        const char *ayrac=" ";
        char *kelime;
        sehir *dinamikk = (sehir*)malloc(sizeof(sehir));
        while(root2!=NULL)
        {
            root2->komsu=NULL;
            kelime = strtok(root2 -> veri, ayrac);
            while (kelime != NULL)
            {
                while(root3!=NULL)
                {
                    if(strcmp(root3->adi, kelime)==0)
                    {
                        dinamikk->plaka=root3->plaka;
                        strcpy(dinamikk->adi, root3->adi);
                        strcpy(dinamikk->bolge, root3->bolge);
                        root2->komsu=komsuEkle (root2, dinamikk);
                        root3=root;
                        break;
                    }
                    root3=root3->next;
                }
                kelime = strtok(NULL, ayrac);
            }
            if(root2->next==NULL)
                root4=root2;
            root2=root2->next;
        }

        int marker=sehirSayisi;
        while(marker>0)
        {
            if(marker>1)
                root4=root4->prev;
            marker--;
        }

        return root4;
    }
}

char*boslukYap(char*s, char cop) // ikinci parametredeki karakteri, boþluk karakterine çeviriliyor.
{
    int i;
    for(i=0; i<strlen(s); i++)
    {
        if(s[i]==cop)
            s[i]=' ';
    }
    return s;
}

int komsuSayisiniBul(char*s) // boþluk sayýsý ile ilk okumada komþu sayýlarý bulunuyor.
{
    int i;
    int j=0;
    for(i=0; i<strlen(s); i++)
    {
        if ((s[i]==' '))
        {
            j++;
        }
    }
    int komsuSayisi=j-2;
    return komsuSayisi;
}

sehir *sehirEkle(sehir *r, sehir *dinamik)
{
    if(r==NULL)  // linklist bossa
    {
        r = (sehir *)malloc(sizeof(sehir));
        r -> next = NULL;
        r -> prev = NULL;
        r -> plaka = dinamik -> plaka;
        r -> komsuSayisi = dinamik -> komsuSayisi;
        strcpy(r -> adi, dinamik -> adi);
        strcpy(r -> bolge, dinamik -> bolge);
        strcpy(r -> veri, dinamik -> veri);
        return r;
    }
    if((r->plaka) > (dinamik->plaka)) // ilk elemandan kucuk durumu
    {
        // root degisiyor
        sehir * temp = (sehir*)malloc(sizeof(sehir));
        temp -> plaka = dinamik -> plaka;
        temp -> komsuSayisi = dinamik -> komsuSayisi;
        strcpy(temp -> adi, dinamik -> adi);
        strcpy(temp -> bolge, dinamik -> bolge);
        strcpy(temp -> veri, dinamik -> veri);
        temp -> next =r;
        r->prev = temp;
        return temp;
    }
    sehir * iter = r;
    while(iter->next != NULL && (iter->next->plaka) < (dinamik->plaka))
    {
        iter= iter->next;
    }
    sehir * temp = (sehir*)malloc(sizeof(sehir));
    temp->next = iter -> next;
    iter -> next = temp;
    temp->prev = iter;

    if(temp->next != NULL)
    {
        temp->next->prev = temp;
    }

    temp -> plaka = dinamik -> plaka;
    temp -> komsuSayisi = dinamik -> komsuSayisi;
    strcpy(temp -> adi, dinamik -> adi);
    strcpy(temp -> bolge, dinamik -> bolge);
    strcpy(temp -> veri, dinamik -> veri);
    return r;
}

komsu * komsuEkle (sehir * r, sehir * dinamikKomsu)
{
    if(r->komsu==NULL)  // linklist bossa
    {
        komsu* yeni = (komsu *)malloc(sizeof(komsu));
        r->komsu=yeni;
        r->komsu->next = NULL;
        r->komsu->plaka = dinamikKomsu->plaka;
        strcpy(r->komsu->adi, dinamikKomsu->adi);
        strcpy(r->komsu->bolge, dinamikKomsu->bolge);
        return r->komsu;
    }
    if(r->komsu->plaka > dinamikKomsu->plaka)  // ilk elemandan kucuk durumu
    {
        // root degisiyor
        komsu* yeni = (komsu*)malloc(sizeof(komsu));
        komsu* temp=yeni;
        temp -> plaka = dinamikKomsu->plaka;
        strcpy(temp->adi, dinamikKomsu->adi);
        strcpy(temp->bolge, dinamikKomsu->bolge);
        temp -> next =r->komsu;
        return temp;
    }
    komsu * iter = (komsu*)malloc(sizeof(komsu));
    iter = r->komsu;
    while(iter->next != NULL && iter->next->plaka < dinamikKomsu->plaka)
    {
        iter= iter->next;
    }
    komsu * temp = (komsu*)malloc(sizeof(komsu));
    temp->next = iter -> next;
    iter -> next = temp;
    temp -> plaka = dinamikKomsu->plaka;
    strcpy(temp->adi, dinamikKomsu->adi);
    strcpy(temp->bolge, dinamikKomsu->bolge);
    return r->komsu;
}

void dosyayaYazdir(sehir *r)
{
    fp=fopen("cikti.txt","w");
    if (fp == NULL)
    {
        printf("\n Dosya acilamadi\n");
        fclose(fp);
    }

    while(r!=NULL)
    {
        fprintf(fp, "%d | %s | %s ==>> ", r->plaka, r->bolge, r->adi);
        while(r->komsu!=NULL)
        {
            fprintf(fp, "| %d ", r->komsu->plaka);
            r->komsu=r->komsu->next;
        }
        fprintf(fp, "\n");
        r=r->next;
    }

    fclose(fp);
}

int sayiMi (char str[])
{
    int uzunluk=strlen(str);
    int i;
    int sayi=0, harf=0;
    for(i=0; i < uzunluk; i++ )
    {
        if(isdigit(str[i]))
            sayi=1;

        if(isalpha(str[i]))
            harf=1;
    }

    if(sayi==1 && harf==0)
        return 1; //sayýdýr

    if(harf==1 && sayi==0)
        return 2; //isimdir

}

sehir *sehirAra(sehir *r)
{
    sehir *degisken=r;
    int i=1;
    int plaka;
    char adi[100];
    char girdi[100];
    printf("Goruntulemek istediginiz sehrin adi veya plakasini yazin:\n");
    scanf("%s", &girdi);
    printf("\n");
    int sonuc=sayiMi(girdi);
    bool durum=false;

    if(sonuc==1)
    {
        sscanf(girdi, "%d", &plaka);
        while(r!=NULL)
        {
            if(plaka==r->plaka)
            {
                durum=true;
                printf("Plaka no: %d | Bolge adi: %s | Sehir adi: %s | Komsu sayisi: %d\n", r->plaka, r->bolge, r->adi, r->komsuSayisi);
                komsu * r2=r->komsu;
                while(r2!=NULL)
                {
                    printf("%d.Komsu: Plaka no: %d | Bolge adi: %s | Sehir adi: %s\n", i, r2->plaka, r2->bolge, r2->adi);
                    r2=r2->next;
                    i++;
                }
                i=1;
            }
            r=r->next;
        }
        printf("\n");
    }

    else if(sonuc==2)
    {
        sscanf(girdi, "%s", &adi);
        while(r!=NULL)
        {
            if(strcmp(r->adi, adi)==0)
            {
                durum=true;
                printf("Plaka no: %d | Bolge adi: %s | Sehir adi: %s | Komsu sayisi: %d\n", r->plaka, r->bolge, r->adi, r->komsuSayisi);
                komsu * r2=r->komsu;
                while(r2!=NULL)
                {
                    printf("%d.Komsu: Plaka no: %d | Bolge adi %s | Sehir adi %s\n", i, r2->plaka, r2->bolge, r2->adi);
                    r2=r2->next;
                    i++;
                }
                i=1;
            }
            r=r->next;
        }
        printf("\n");
    }

    else
        printf("Hatalý bir giris yaptiniz!\n");

    if(durum==false)
    {
        int secim;
        printf("Sehir listede yok. Yeni bir sehir ekleyecek misiniz? (Evetse 1'e, Hayirsa 2'ye basin.) : ");
        scanf("%d", &secim);
        if(secim==1)
            degisken=yeniSehirEkle(degisken);
    }
    return degisken;
}

void bolgeyleListele(sehir *r)
{
    int i=1;
    bool eslesme=false;
    char adi[100];
    printf("Marmara: MA\nAkdeniz: AK\nEge: EG\nKaradeniz: KA\nIc Anadolu: IA\nDogu Anadolu: DA\nGuneydogu Anadolu: GA\n");
    printf("Sehirlerini siralamak istediginiz bolgenin kodunu yaziniz.\n");
    scanf("%s", &adi);
    while(r!=NULL)
    {
        if(strcmp(r->bolge, adi)==0)
        {
            eslesme=true;
            printf("%d - Plaka no: %d | Bolge adi: %s | Sehir adi: %s | Komsu sayisi: %d\n", i, r->plaka, r->bolge, r->adi, r->komsuSayisi);
            i++;
        }
        r=r->next;
    }
    if(eslesme==false)
        printf("Hatali bir giris yaptiniz!\n");
    printf("\n");
}

void komsuSayisiylaListele(sehir *r)
{
    bool eslesme=false;
    int i=1;
    int min, max;
    printf("Minimum komsu sayisini girin:\n");
    scanf("%d", &min);
    printf("Maksimum komsu sayisini girin:\n");
    scanf("%d", &max);
    printf("\n");

    while(r!=NULL)
    {
        if(r->komsuSayisi>(min-1) && r->komsuSayisi<(max+1))
        {
            eslesme=true;
            printf("%d - Plaka no: %d | Bolge adi: %s | Sehir adi: %s | Komsu sayisi: %d\n", i, r->plaka, r->bolge, r->adi, r->komsuSayisi);
            i++;
        }
        r=r->next;
    }

    if(eslesme==false)
        printf("Hatali bir giris yaptiniz veya bu sartlara uyan bir sehir mevcut degil!\n");
    printf("\n");
}

sehir *yeniSehirEkle(sehir *r)
{
    sehir * r1=r;
    sehir * r2=r;
    bool durum=true;
    int plaka;
    int komsuSayisi;
    char sehirAdi[100];
    char bolgeAdi[100];
    printf("Ekleyeceginiz sehrin plakasini girin: ");
    scanf("%d", &plaka);
    while(r1!=NULL)
    {
        if(r1->plaka==plaka)
        {
            printf("Bu plaka ile kayitli bir sehir zaten bulunmaktadir.\n");
            durum=false;
            break;
        }
        r1=r1->next;
    }

    if(durum==true)
    {
        printf("Ekleyeceginiz sehrin ismini girin: ");
        scanf("%s", &sehirAdi);
        while(r2!=NULL)
        {
            if(strcmp(r2->adi, sehirAdi)==0)
            {
                printf("Bu sehir adi ile kayitli bir sehir zaten bulunmaktadir.\n");
                durum=false;
                break;
            }
            r2=r2->next;
        }
    }

    if(durum==true)
    {
        printf("Ekleyeceginiz sehrin bolge kodunu girin: ");
        scanf("%s", &bolgeAdi);
        printf("Ekleyeceginiz sehrin komsu sayisini girin: ");
        scanf("%d", &komsuSayisi);
        int komsular [komsuSayisi];

        sehir *eklenen = (sehir*)malloc(sizeof(sehir));
        eklenen->plaka=plaka;
        eklenen->komsuSayisi=komsuSayisi;
        strcpy(eklenen->bolge, bolgeAdi);
        strcpy(eklenen->adi, sehirAdi);

        r = sehirEkle(r, eklenen);

        int marker;
        int i;
        int komsuPlaka;
        char komsuAdi[100];
        char komsuBolge[10];
        sehir *dinamikKomsu = (sehir*)malloc(sizeof(sehir));

        marker=0;
        while(r!=NULL)
        {
            if(r->plaka==plaka)
            {
                r->komsu=NULL;
                for(i=0; i<komsuSayisi; i++)
                {
                    printf("%d. Komsunun plakasini giriniz: ", i+1);
                    scanf("%d", &komsuPlaka);
                    komsular[i]=komsuPlaka;
                    printf("%d. Komsunun adini giriniz: ", i+1);
                    scanf("%s", &komsuAdi);
                    printf("%d. Komsunun bolge kodunu giriniz: ", i+1);
                    scanf("%s", &komsuBolge);

                    dinamikKomsu->plaka=komsuPlaka;
                    strcpy(dinamikKomsu->adi, komsuAdi);
                    strcpy(dinamikKomsu->bolge, komsuBolge);
                    r->komsu=komsuEkle(r, dinamikKomsu);
                }

                break;
            }
            marker++;
            r=r->next;
        }

        while(marker>0)
        {
            r=r->prev;
            marker--;
        }
        sehirSayisi++;

        int k, j, tmp;
        for(k=1; k<komsuSayisi; k++)
        {
            for(j=0; j<komsuSayisi-1; j++)
            {
                if(komsular[j]>komsular[j+1])
                {
                    tmp=komsular[j];
                    komsular[j]=komsular[j+1];
                    komsular[j+1]=tmp;
                }
            }
        }

        marker=0;
        for(i=0; i<komsuSayisi; i++)
        {
            while(r!=NULL)
            {
                if(r->plaka==komsular[i])
                {
                    r->komsu=komsuEkle(r, eklenen);
                    r->komsuSayisi++;
                    break;
                }
                marker++;
                r=r->next;
            }
        }

        while(marker>0)
        {
            r=r->prev;
            marker--;
        }

        return r;
    }
}

sehir *sehirSil(sehir *r, int x)
{
    sehir * temp;
    sehir * iter = r;
    if(r->plaka == x)
    {
        temp = r;
        r=r->next;
        r->prev=NULL;
        free(temp);
        return r;
    }

    while (iter->next != NULL && iter->next->plaka != x)
    {
        iter = iter ->next;
    }
    if(iter->next == NULL)
    {
        return r;
    }
    temp = iter->next;
    iter->next = iter->next->next;
    free (temp);
    if(iter->next != NULL)
        iter->next->prev = iter;
    return r;
}

komsu *komsuSil(sehir *r, int x)
{
    komsu* temp;
    komsu* iter = r->komsu;
    if(r->komsu->plaka == x)
    {
        temp = r->komsu;
        r->komsu = r->komsu->next;
        free(temp);
        return r->komsu;
    }
    while (iter->next != NULL && iter->next->plaka != x)
    {
        iter = iter ->next;
    }
    if(iter->next == NULL)
    {
        return r->komsu;
    }
    temp = iter->next;
    iter->next = iter->next->next;
    free (temp);
    return r->komsu;
}

sehir *silMenu(sehir *r)
{
    sehir *r2=r;
    sehir *r3=r;
    int i=0;
    int komsuSayisi;
    int plaka;
    int marker=0;
    int komsular[100];
    bool durum=false;

    printf("Silmek istediginiz sehrin plakasini girin:\n");
    scanf("%d", &plaka);
    while(r2!=NULL)
    {
        if(r2->plaka==plaka)
        {
            durum=true;
            break;
        }
        r2=r2->next;
    }

    if(durum==false)
        printf("Boyle bir sehir bulunmamaktadir.\n");

    if(durum==true)
    {
        while(r3!=NULL)
        {
            if(r3->plaka==plaka)
            {
                komsuSayisi=r3->komsuSayisi;
                while(r3->komsu!=NULL)
                {
                    komsular[i]=r3->komsu->plaka;
                    i++;
                    r3->komsu=r3->komsu->next;
                }
            }
            r3=r3->next;
        }

        int k, j, tmp;
        for(k=1; k<komsuSayisi; k++)
        {
            for(j=0; j<komsuSayisi-1; j++)
            {
                if(komsular[j]>komsular[j+1])
                {
                    tmp=komsular[j];
                    komsular[j]=komsular[j+1];
                    komsular[j+1]=tmp;
                }
            }
        }

        r=sehirSil(r, plaka);

        for(i=0; i<komsuSayisi; i++)
        {
            while(r!=NULL)
            {
                if(r->plaka==komsular[i])
                {
                    r->komsu=komsuSil(r, plaka);
                    r->komsuSayisi--;
                    break;
                }
                marker++;
                r=r->next;
            }
        }

        while(marker>0)
        {
            r=r->prev;
            marker--;
        }
    }

    return r;
}

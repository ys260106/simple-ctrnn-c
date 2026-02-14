//Bu proje yeni diferansiyel denklemi test etmek ve nöron bağlantılarını daha iyi anlamak için yazılmıştır

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include "lnn_cekirdek.h"


float kaos_ihtimal(){
    float kaos = rand() % 100;

    if(kaos < 0){
        kaos * (-1);
    }
    return kaos;
}

float sicaklik_ivme(){
    float ivme = rand() % 40;

    if(ivme < 20 ) ivme = 20;

    return ivme;
}

float buhar_ivme(){
    float ivme = rand() % 8;

    if(ivme < 3) ivme = 3;

    return ivme;
}

float radyasyon_ivme(){
    float ivme = rand() % 20;

    if(ivme < 8) ivme = 8;

    return ivme;
}
float titresim_su_ivme(){
    float ivme = rand() % 8;

    if(ivme < 3) ivme = 3;

    return ivme;
}


float sicaklik_rastgele(float x){

    float sıcaklık;

    if(x < 10){
        sıcaklık = 750;

    }else{
        sıcaklık = 150;
    }
    return sıcaklık;
}

float buhar_basıncı(float x){

    float bar;

    if(x < 10) {
        bar = 120;

    }else{
        bar = 40;

    }

    return bar;
}

float radyasyon_sızıntısı(float x){
    float radyasyon;
    if(x < 10){
        radyasyon = 55;
    }else{
        radyasyon = 2;
    }
    return radyasyon;
}

float tiresim_seviyesi(float x){
    float titresim;
    if(x < 10){
        titresim = 50;
    }else{
        titresim = 10;
    }
    return titresim;
}
float su_seviyesi(float x){
    float su;
    if( x < 10){
        su = 50;
    }else{
        su = 100;
    }
    return su;
}

int main(){
    float dt = 0.1;
    //Sensör noronlarını yazıyoruz
    Lnn_ag *layer_sensor = katman_olustur(5);

    //Analiz öronlarını yazıyoruz
    Lnn_ag *layer_analiz = katman_olustur(3);

    //Ve son olarak 1 tane çıkış karar nöronu
    Lnn_ag *layer_karar = katman_olustur(1);

    //-------------Ayarlamaları Yapıyrouz-----------

    //Sensör noronları daha hızlı tepkı vermesı lazım
    Lnn_Yapilandir(layer_sensor,0 ,5 ,5 ,5 , -2.5);

    //Analiz noronları orta sertlıkte olmalı
    Lnn_Yapilandir(layer_analiz,0 , 3, 4, 2, 0);

    //Çıkış nöron ıse daha sert ve zor ikna olmalı
    Lnn_Yapilandir(layer_karar,0, 1, 7 ,5 , -3);

    //Bu kısımdan Nöron oluşturma kısmına ve yukarı kısımdakı rastgelelik fonksiyonları kısmına kadar yapılan anlatım işlemleri nukleerastgele.c dosyasında
    srand(time(NULL));

    float kaos = kaos_ihtimal();

    float sicaklik =  sicaklik_rastgele(kaos);
    float sicaklik_rastgele_ivme = sicaklik_ivme();

    float buhar = buhar_basıncı(kaos);
    float rastgele_buhar_ivme = buhar_ivme();

    float radyasyon = radyasyon_sızıntısı(kaos);
    float rastgele_radyasyon_ivme = radyasyon_ivme();

    float titresme = tiresim_seviyesi(kaos);
    float rastgele_titresim_ivme = titresim_su_ivme();

    float su_doluluk = su_seviyesi(kaos);
    float rastgele_su_ivme = titresim_su_ivme();


    for(int i = 0; i < 50; i++){

        sicaklik = sicaklik + sicaklik_rastgele_ivme;
        if(sicaklik > 1150){
            sicaklik = 1200;
        }
        buhar = buhar + rastgele_buhar_ivme;
        if(buhar > 200){
            buhar = 200;
        }
        radyasyon = radyasyon + rastgele_radyasyon_ivme;
        if(radyasyon > 500){
            radyasyon = 500;
        }
        titresme = titresme + rastgele_titresim_ivme;
        if(titresme > 100){
            titresme = 100;
        }
        su_doluluk = su_doluluk - rastgele_su_ivme;
        if(su_doluluk < 0){
            su_doluluk = 0;
        }


    //---Normalizasyon İşlemleri---

 float sicaklik_input = sicaklik / 1200;

 float buhar_input = buhar / 200;

 float radyasyon_input = radyasyon / 500;

 float titresme_input = titresme / 100;

 float su_input = 1- (su_doluluk / 100);

//Sırada İnputları nöronlarımızıa vericez;
    layer_sensor->noronlar[0].x = sicaklik_input;
    layer_sensor->noronlar[1].x = buhar_input;
    layer_sensor->noronlar[2].x = radyasyon_input;
    layer_sensor->noronlar[3].x = titresme_input;
    layer_sensor->noronlar[4].x = su_input;

    //Nöron katmanını ateşliyoruz
    Lnn_step(layer_sensor,dt);


    //Sırada Analiz katmanı girişleini ayarlıcaz

    //Bu kısımda analiz katmanının 0. ve 1. nöronu sıcaklık ve buharı analiz edicek
    layer_analiz->noronlar[0].x = (layer_sensor->noronlar[0].y + layer_sensor->noronlar[1].y) / 2.0f;

    //Analiz katmanının 1. nöronu ise radyasyon ve titreşimi analiz edicek
    layer_analiz->noronlar[1].x = (layer_sensor->noronlar[2].y + layer_sensor->noronlar[3].y) / 2.0f;

    //Analiz katmanının 2. nöronu ise su seviyesi ve titreşimi analiz edicek
    layer_analiz->noronlar[2].x = (layer_sensor->noronlar[4].y + layer_sensor->noronlar[3].y) / 2.0f;


    //Sıra analiz katmanını ateşlemekte
    Lnn_step(layer_analiz,dt);


    //En son olarak ise sıra çıkış katmanını ayarlamakta bu kısımda düz ortalama almak yerine ağırlıklı ortalama ile önem derecesine göre ortalama alıcaz

    //Not= oran 1 üzerinden alınmıştır

    //en önemli etken radyasyon ve su olduğu için analiz 0 ın işi olan sıcaklık ve buhar kısmına yüzde 20 lik etken yeterdir
    float analiz_0_katkisi = layer_analiz->noronlar[0].y * 0.20f;

    //radyasyon ve titreşim sıcaklık ve buhardan daha önemlidir
    float analiz_1_katkisi = layer_analiz->noronlar[1].y * 0.30f;

    //ve en önemli kısım su ve titreşim
    float analiz_2_katkisi = layer_analiz->noronlar[2].y * 0.50f;

    //sıra toplama da
    float son_karar_girisi = analiz_0_katkisi + analiz_1_katkisi + analiz_2_katkisi;

    layer_karar->noronlar[0].x = son_karar_girisi;

    //Nöronu ateşliyoruz
    Lnn_step(layer_karar,dt);

    if(i == 0) {
        printf("\nADIM | SICAK   BUHAR   RADYASYON   TİTREŞİM   SU %%    || KARAR\n");
        printf("-------------------------------------------------------------------\n");
    }

    // Verileri tek satırda yazdır
    printf("%-4d | %-8.2f %-8.2f %-9.2f %-10.2f %-8.2f || %.5f\n",
           i,
           layer_sensor->noronlar[0].y,
           layer_sensor->noronlar[1].y,
           layer_sensor->noronlar[2].y,
           layer_sensor->noronlar[3].y,
           layer_sensor->noronlar[4].y,
           layer_karar->noronlar[0].y
    );


    usleep(100000);
    }
}

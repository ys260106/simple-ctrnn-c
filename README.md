# simple-ctrnn-c: Saf C ile Nöronal Dinamik Testi

Bu proje, herhangi bir hazır yapay zeka kütüphanesi (PyTorch, TensorFlow vb.) kullanılmadan, **saf C (Pure C)** dili ile sıfırdan geliştirilmiş, Sürekli Zamanlı Yinelemeli Sinir Ağı (CTRNN) tabanlı deneysel bir simülasyon çalışmasıdır.

##  Projenin Amacı

Bu çalışmanın ana amacı, **diferansiyel denklemler (ODE)** ile modellenen nöronların, bir ağ yapısı içinde birbirleriyle nasıl etkileşime girdiğini ve sinyal iletiminin zaman içindeki davranışını (dinamiğini) düşük seviyeli (low-level) bir mimaride test etmektir.

Proje, C programlama dilindeki bellek yönetimi (**pointers**) ve veri yapıları (**structs**) kullanılarak oluşturulan bir "Sinir Ağı Motoru"nun kararlılığını ölçmeyi hedefler.

##  Ağ Mimarisi ve Test Senaryosu

Ağın sinyal işleme ve karar verme yeteneğini gözlemlemek amacıyla, sentetik bir **"Nükleer Reaktör Kontrol Sistemi"** senaryosu simüle edilmiştir.

Sistem, **5-3-1** topolojisine sahip 3 katmanlı bir yapı üzerine kurulmuştur:

1.  **Sensör Katmanı (5 Nöron):** Dış dünyadan gelen ham verileri (Sıcaklık, Buhar Basıncı, Radyasyon, Titreşim, Su Seviyesi) sisteme dahil eder.
2.  **Analiz Katmanı (3 Nöron):** Sensörlerden gelen verileri birleştirerek (Sensor Fusion) ara değerlendirme yapar.
3.  **Karar Katmanı (1 Nöron):** Analiz katmanından gelen ağırlıklı sinyallere ve kendi zaman sabitine ($\tau$) göre sistemin güvenli olup olmadığına karar verir.

##  Teknik Detaylar

* **Dil:** C (Standart Kütüphaneler dışında bağımlılık yoktur).
* **Matematiksel Model:** Leaky Integrate-and-Fire nöron modeli.
* **Çözücü (Solver):** Euler İntegrasyon Yöntemi (Birinci dereceden ODE çözümü).
* **Veri Yapıları:** `typedef` ve `struct` yapıları ile nesne tabanlı yaklaşıma benzer modüler bir yapı kurulmuştur.
* **Hafıza Yönetimi:** Nöron bağlantıları ve katmanlar arası veri akışı pointer aritmetiği ile yönetilmiştir.

### State Space (Durum Uzayı) ve Hata Toleransı
Sistem, karar mekanizması olarak Liquid Neural Network (LNN) kullanırken, veri güvenilirliğini sağlamak için entegre bir **State Space (Durum Uzayı)** modülü barındırır.

Bu mimaride:
* Sensör verileri, klasik if-else blokları yerine **5 boyutlu bir vektör uzayında** analiz edilir.
* Öklid mesafesi (Euclidean Distance) yöntemiyle matematiksel **Anomali Tespiti** yapılır.
* Bu sayede sistem, tekil sensör arızalarına veya manipülasyonlara (Sabotaj Testi ile doğrulanmıştır) karşı hata toleransına sahiptir.
// Sabotaj modunu aktif etmek için O yerine 1 yapın ve yeniden derleyip çalıştırın
#define TEST_MODU 1

##  Derleme ve Çalıştırma

Projeyi derlemek için herhangi bir C derleyicisi (GCC, Clang vb.) yeterlidir.

```bash
# Projeyi derle
gcc main.c lnn_cekirdek.c -o lnn_test -lm

# Çalıştır
./lnn_test

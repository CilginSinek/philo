# Philosophers - GitHub Actions Test Suite

Bu proje için kapsamlı bir test suite oluşturulmuştur. Her push ve pull request'te otomatik olarak çalışır.

## 🧪 Test Kategorileri

### 1. **Temel Fonksiyonellik Testleri** (Mandatory & Bonus)
- ✅ Tek filozof testi (ölmeli)
- ✅ Kimse ölmemeli testleri
- ✅ Ölüm tespiti testleri
- ✅ Yemek sayısı tamamlama testleri
- ✅ Büyük sayıda filozof testleri (100 filozof)
- ✅ Edge case testleri

### 2. **Valgrind - Memory Leak Testleri**
- 🔍 Heap memory leak kontrolü
- 🔍 Memory corruption kontrolü
- 🔍 Use-after-free kontrolü
- 🔍 Double-free kontrolü

### 3. **Helgrind - Data Race Testleri**
- 🔒 Thread data race kontrolü
- 🔒 Mutex kullanım kontrolü
- 🔒 Concurrent access kontrolü

### 4. **DRD - Thread Error Testleri**
- 🧵 Thread synchronization kontrolü
- 🧵 Deadlock kontrolü
- 🧵 Lock order kontrolü

## 📊 Test Senaryoları

### Mandatory (philo) Testleri:

| Test | Parametreler | Beklenen Sonuç | Timeout |
|------|-------------|----------------|---------|
| Tek filozof | `1 800 200 200` | Ölmeli | 5s |
| Hiç ölüm yok | `4 410 200 200` | Sonsuz döngü | 10s |
| Hiç ölüm yok | `5 800 200 200` | Sonsuz döngü | 10s |
| Birisi ölmeli | `5 310 200 200` | Ölüm tespit | 5s |
| Yemek sayısı | `5 800 200 200 7` | Tamamlanmalı | 30s |
| Yemek sayısı | `4 410 200 200 10` | Tamamlanmalı | 30s |
| Büyük sayı | `100 800 200 200` | Sonsuz döngü | 10s |
| Büyük sayı+eat | `100 800 200 200 5` | Tamamlanmalı | 60s |
| Hızlı ölüm | `4 200 205 200` | Ölüm tespit | 5s |
| İki filozof | `2 410 200 200` | Sonsuz döngü | 10s |

### Bonus (philo_bonus) Testleri:

Aynı test senaryoları bonus versiyonu için de uygulanır.

## 🚀 Nasıl Kullanılır?

### Yerel Test Çalıştırma:

```bash
# Mandatory testleri
cd philo
make
./philo 5 800 200 200 7

# Valgrind ile memory leak kontrolü
valgrind --leak-check=full ./philo 5 800 200 200 7

# Helgrind ile data race kontrolü
valgrind --tool=helgrind ./philo 5 800 200 200 7

# DRD ile thread error kontrolü
valgrind --tool=drd ./philo 5 800 200 200 7
```

```bash
# Bonus testleri
cd philo_bonus
make
./philo_bonus 5 800 200 200 7

# Valgrind testleri
valgrind --leak-check=full ./philo_bonus 5 800 200 200 7
valgrind --tool=helgrind ./philo_bonus 5 800 200 200 7
valgrind --tool=drd ./philo_bonus 5 800 200 200 7
```

### GitHub Actions'da Otomatik Test:

1. Kodu GitHub'a push et:
```bash
git add .
git commit -m "Add philosophers implementation"
git push origin main
```

2. GitHub repository'nin **Actions** sekmesine git
3. Workflow'un çalışmasını izle
4. Her test adımının sonucunu gör

## 📋 Test Çıktısı Yorumlama

### ✅ Başarılı Test:
```
✅ No memory leaks detected
✅ No data races detected
✅ Test completed
```

### ❌ Başarısız Test:
```
❌ Data races or errors detected
ERROR SUMMARY: 4 errors from 4 contexts
```

### ⚠️ Timeout (Beklenen):
```
⚠️ Timeout (expected for infinite simulation)
```
- Eat count verilmediğinde program sonsuz döngüde kalmalı
- Bu normal ve beklenen davranıştır

## 🔧 Test Timeout Süreleri

- **Ölüm testleri**: 5 saniye
- **Kısa sonsuz döngü testleri**: 10 saniye
- **Eat count testleri**: 30 saniye
- **Büyük filozof sayısı testleri**: 60 saniye
- **Valgrind/Helgrind/DRD testleri**: 90 saniye

## 📝 Notlar

1. **Timeout'lar uzun tutulmuştur** çünkü Valgrind programı çok yavaşlatır
2. **Helgrind/DRD** process-based semaphore kullanımında bazı false-positive'ler verebilir (bonus için normal)
3. **Eat count** testlerinde program düzgün sonlanmalı, timeout olmamalı
4. **Infinite loop** testleri timeout ile biter, bu normaldir

## 🐛 Debug İpuçları

### Memory Leak Bulunursa:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./philo 5 800 200 200 7
```

### Data Race Bulunursa:
```bash
valgrind --tool=helgrind --history-level=approx ./philo 5 800 200 200 7
```

### Deadlock Şüphesi:
```bash
valgrind --tool=drd --check-stack-var=yes ./philo 5 800 200 200 7
```

## 📚 Kaynaklar

- [Valgrind Documentation](https://valgrind.org/docs/manual/manual.html)
- [Helgrind Manual](https://valgrind.org/docs/manual/hg-manual.html)
- [DRD Manual](https://valgrind.org/docs/manual/drd-manual.html)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)

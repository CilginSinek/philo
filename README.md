# 🍝 Philosophers

![Tests](https://github.com/cilginsinek/philo/workflows/Philosophers%20Tests/badge.svg)
![Platform](https://img.shields.io/badge/platform-Linux-blue)
![Language](https://img.shields.io/badge/language-C-brightgreen)
![License](https://img.shields.io/badge/license-MIT-orange)

42 Okulu **Philosophers** projesi - Dining Philosophers probleminin thread ve process tabanlı implementasyonları.

## 📖 Proje Hakkında

Bu proje, klasik **Dining Philosophers** senkronizasyon problemini çözer:

- 🍝 Filozoflar bir masa etrafında oturur
- 🍴 Her filozofun arasında bir çatal vardır
- 🤔 Filozoflar düşünür, yer ve uyur
- ⏱️ Belirli bir süre yemek yemezlerse ölürler
- 🔒 Deadlock ve data race olmamalı

## 🚀 Özellikler

### Mandatory (philo)
- ✅ Thread-based implementation
- ✅ Mutex synchronization
- ✅ No data races
- ✅ No memory leaks
- ✅ Death detection in < 10ms

### Bonus (philo_bonus)
- ✅ Process-based implementation
- ✅ Semaphore synchronization
- ✅ No data races
- ✅ No memory leaks
- ✅ Death detection in < 10ms

## 🛠️ Kurulum ve Kullanım

### Derleme

```bash
# Mandatory version
cd philo
make

# Bonus version
cd philo_bonus
make
```

### Kullanım

```bash
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat (optional)]
```

**Parametreler:**
- `number_of_philosophers`: Filozof sayısı (ve çatal sayısı)
- `time_to_die`: Ölüm süresi (ms) - Bu süre içinde yemek yemezse ölür
- `time_to_eat`: Yemek yeme süresi (ms)
- `time_to_sleep`: Uyku süresi (ms)
- `number_of_times_each_philosopher_must_eat`: (Opsiyonel) Her filozofun kaç kez yemesi gerektiği

### Örnekler

```bash
# 5 filozof, kimse ölmemeli
./philo 5 800 200 200

# 4 filozof, 10 kez yemeli ve programdan çıkmalı
./philo 4 410 200 200 10

# 1 filozof, ölmeli (tek çatal var)
./philo 1 800 200 200

# 5 filozof, birisi ölmeli
./philo 5 310 200 200
```

## 🧪 Test Etme

### Manuel Test

```bash
# Memory leak kontrolü
valgrind --leak-check=full ./philo 5 800 200 200 7

# Data race kontrolü
valgrind --tool=helgrind ./philo 5 800 200 200 7

# Thread error kontrolü
valgrind --tool=drd ./philo 5 800 200 200 7
```

### Otomatik Test

Proje GitHub Actions ile otomatik test edilir. Her push'ta:
- ✅ 40+ farklı test senaryosu çalışır
- ✅ Memory leak kontrolü yapılır
- ✅ Data race kontrolü yapılır
- ✅ Thread synchronization kontrolü yapılır

Detaylı test dokümantasyonu için: [TEST_DOCUMENTATION.md](TEST_DOCUMENTATION.md)

## 📊 Test Sonuçları

GitHub repository'nin **Actions** sekmesinden test sonuçlarını görebilirsiniz.

### Test Kategorileri

| Kategori | Test Sayısı | Açıklama |
|----------|------------|----------|
| Basic Functionality | 20 | Temel fonksiyonellik testleri |
| Memory Leaks | 6 | Valgrind memory leak testleri |
| Data Races | 4 | Helgrind data race testleri |
| Thread Errors | 4 | DRD thread error testleri |

## 🏗️ Proje Yapısı

```
philo/
├── philo/                  # Mandatory (thread-based)
│   ├── events.c           # Filozof aksiyonları
│   ├── helper.c           # Yardımcı fonksiyonlar
│   ├── init.c             # Initialization
│   ├── main.c             # Main ve monitoring
│   ├── utils.c            # Utility fonksiyonları
│   ├── philo.h            # Header file
│   └── Makefile
├── philo_bonus/           # Bonus (process-based)
│   ├── init_bonus.c       # Initialization
│   ├── libft_bonus.c      # Library fonksiyonları
│   ├── main_bonus.c       # Main ve monitoring
│   ├── philo_helper_bonus.c
│   ├── philo_routine_bonus.c
│   ├── print_bonus.c
│   ├── thread_helper_bonus.c
│   ├── ownsems_bonus.c
│   ├── philo_bonus.h
│   └── Makefile
└── .github/
    └── workflows/
        └── test.yml       # GitHub Actions test workflow
```

## 🔧 Teknik Detaylar

### Thread Safety (Mandatory)
- Mutex'ler ile kritik bölge koruması
- Atomic flag kontrolü ile data race önleme
- Death monitor thread'i ile ölüm tespiti

### Process Safety (Bonus)
- Named semaphore'ler ile process senkronizasyonu
- Fork-based process creation
- Inter-process communication

### Optimizasyonlar
- Efficient death detection (< 10ms)
- Minimal CPU usage
- No busy-waiting
- Clean resource cleanup

## 🐛 Bilinen Sorunlar ve Çözümler

### Valgrind Uyarıları (Bonus)
- Helgrind process-based semaphore'lerde bazı false-positive'ler verebilir
- Bu normal ve beklenen davranıştır
- Gerçek data race'ler düzeltilmiştir

### Timeout Davranışı
- Eat count belirtilmediğinde program sonsuz döngüde kalır (normal)
- Eat count ile program otomatik sonlanmalıdır

## 📚 Kaynaklar

- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [POSIX Semaphores](https://man7.org/linux/man-pages/man7/sem_overview.7.html)
- [Valgrind Documentation](https://valgrind.org/docs/)

## 👨‍💻 Geliştirici

**iduman** - [cilginsinek](https://github.com/cilginsinek)

## 📝 Lisans

Bu proje MIT lisansı altındadır.

---

**Not**: Bu proje 42 İstanbul müfredatının bir parçasıdır.

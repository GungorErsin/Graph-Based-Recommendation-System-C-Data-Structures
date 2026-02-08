#ifndef PROJE_H_
#define PROJE_H_

#define MAX_USERS 944
#define MAX_MOVIES 1683
#define MAX_INTERACTIONS 750
#define MAX_GENRES 19
#define MAX_MOVIE_TITLE_LENGTH 100
#define MAX_OCCUPATION_TITLE_LENGTH 16

/*-----Structlarının tanımı-----*/

/*Edge'lerin(kenarların) struct'ı*/
typedef struct {
    int itemId;
    int rating;
}Interaction;

/*User struct'ı*/
typedef struct {
    int userId;
    Interaction* interactions;
    int interactionCount;
}User;

/*Movie struct'ı*/
typedef struct {
    int movieId;
    char* movieTitle;
    int genres[MAX_GENRES];
    Interaction* interactions;
    int interactionCount;
}Movie;

/*-----STRUCTLAR İLE İLGİLİ FONKSİYONLAR-----*/

/*Interaction yaratma fonksiyonu*/
Interaction* createInteraction();

/*Parametre olarak verilen bilgilerle User struct'ı yaratır ve adresini döndürür.*/
User* createUser(int userId);//, int age, const char gender, const char* occupation, int zipcode

/*Parametre olarak verilen bilgilerle Movie struct'ı yaratır ve adresini döndürür.*/
Movie* createMovie(int movieId, const char* movieTitle, int* genres);

/*userId'ye sahip User'ın adresini döndürür.
  users main'de User* users[MAX_USERS] şeklinde tanımlıdır.(Her bir user'ın adresini tutan tablodur)*/
User* getUser(int userId, User** users);

/*MovieId'ye sahip Movie'nin adresini döndürür.
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
Movie* getMovie(int movieId, Movie** movies);

/*-----ML-100K ADLI KLASÖRDEN VERİLE OKUMA İLE İLGİLİ FONKSİYONLAR-----*/

/*Parametre olarak verilen filename dosyasını okuyup ordaki movie verilerini
  yine parametre olarak verilen movies tablosuna kaydeder.
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
void loadMovieData(const char* filename, Movie** movies);

/*Parametre olarak verilen filename dosyasını okuyup ordaki user verilerini
  yine parametre olarak verilen users tablosuna kaydeder.
  users main'de User* users[MAX_USERS] şeklinde tanımlıdır.(Her bir user'ın adresini tutan tablodur)*/
void loadUserData(const char* filename, User** users);

/*Parametre olarak verilen filename dosyasını okuyup ordaki verilerle 
  user'lar ve movie'ler arasındaki edge'leri oluşturur.
  users main'de User* users[MAX_USERS] şeklinde tanımlıdır.(Her bir user'ın adresini tutan tablodur)
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
void loadInteractionData(const char* filename, User** users, Movie** movies);

/*User ve Movie structları içinde tanımlı olan Interaction pointer'ı için
  ayrılan fazla yerleri realloc ile fonksiyonu iade eder
  users main'de User* users[MAX_USERS] şeklinde tanımlıdır.(Her bir user'ın adresini tutan tablodur)
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
void reallocation(User** users, Movie** movies);

/*-----BİRİNCİ ALGORİTMA-----*/

/*1 ile 1682 arasinda rastgele sayi üretir(unwatchedMovieId fonksiyonunda
  kullanılacak)*/
int createRandomNumber();

/*Parametre olarak verilen user'ın izlemediği film ID'lerinden birini döndürür.
  Recursive bir şekilde çalışır(suggestRandomMovie fonksiyonunda kullanılacak)*/
int unwatchedMovieId(User* user);

/*Parametre olarak verilen user'a, parametre olarak verilen suggestionNumber kadar film önerir.
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
void suggestRandomMovies(User* user, Movie** movies, int suggestionNumber);

/*-----İKİNCİ ALGORİTMA-----*/

/*Parametre olarak verilen user filmi izlediyse 1 döndürür, filmi izlemediyse 0 döndürür*/
int isUserWatchedThis(User* user, int movieId);

/*Parametre olarak verilen user'a en çok izlenen filmleri önerir.
  (En çok izlenen filmleri değerlendirme sayısı 300'den fazla olan filmer olarak düşündüm)
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
void suggestTheMostWatchedMovies(User* user, Movie** movies, int suggestionNumber);

/*-----ÜÇÜNCÜ ALGORİTMA-----*/

/*Parametre olarak verilen user'ın tüm bağlantılarını dolaşır ve en çok aynı film 
  izlediği user'ı bulur ve ID'sini döndürür.(suggestMovieWithAnotherUser fonksiyonunda kullanılacak)
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
int findUser(User* user, Movie** movies);

/*Parametre olarak verilen user'ın en çok aynı film izlediği user'ı bulup onun en çok beğendiği filmleri user'a önerir.
  (Maximum parametre olarak verilen suggestionNumber kadar olacak şekilde)
  users main'de User* users[MAX_USERS] şeklinde tanımlıdır.(Her bir user'ın adresini tutan tablodur)
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
void suggestMovieWithAnotherUser(User* user, User** users, Movie** movies, int suggestionNumber);

/*-----DÖRDÜNCÜ ALGORİTMA-----*/

/*Parametre olarak verilen user'a en yakın ağırlıklı uzaklığa sahip filmleri önerir. (Maximum parametre 
  olarak verilen suggesitonNumber kadar olacak şekilde).
  users main'de User* users[MAX_USERS] şeklinde tanımlıdır.(Her bir user'ın adresini tutan tablodur)
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
void suggestMovieWithTheLowestWeightedPathLength(User* user, User** users, Movie** movies, int suggestionNumber);

/*-----BEŞİNCİ ALGORİTMA-----*/

/*Parametre olarak verilen user'ın beğendiği filmleri(4 veya 5 puan verdiği filmler) bulur
  ve bu filmlere bakarak user'ın sevdiği film türünü bulur. Bu türde olupta 150 veya daha fazla 
  değerlendirilen filmlerin arasından izlemediği filmler varsa onları önerir(Maximum parametre olarak
  verilen suggesitonNumber kadar olacak şekilde).
  movies main'de Movie* movies[MAX_MOVİES] şeklinde tanımlıdır.(Her bir movie'nin adresini tutan tablodur)*/
void suggestMovieWithTheMostWatchedGenre(User* user, Movie** movies, int suggestionNumber);

#endif 
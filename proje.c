#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "proje.h"

/*-----STRUCTLAR İLE İLGİLİ FONKSİYONLAR-----*/

Interaction* createInteraction(){
    Interaction* interaction = (Interaction*)malloc(MAX_INTERACTIONS*(sizeof(Interaction)));
    /*Yerin ayrılıp ayrılmadığının kontrolü*/
    if(interaction == NULL){
        printf("HATA!!!BELLEKTE YER AYRILAMADI\n");
        exit(1);
    }
    return interaction;
}

User* createUser(int userId){
    /*userId'ye sahip user yaratma ve yaratılıp yaratılmadığını kontrol etme */
    User* user = (User*)malloc(sizeof(User));
    if(user == NULL){
        printf("user %d icin hafizada gerekli yer ayrilamadi\n",userId);
        return NULL;
    }
    /*user eğer yaratıldıysa bilgilerini doldurma*/
    else{
        user->userId = userId;
        /*user'ın etkileşimde olduğu movie'leri kaydemek için gerekli yeri ayırma
          (burda her user için hafızada eşit miktarda(gereğinden fazla) yer 
          ayrılacak ama ilerde her user için reallocation fonksiyonu ile gereken 
          miktarda hafıza tahsis edilecektir.)*/
        user->interactionCount = 0;
        user->interactions = createInteraction();
        for(int i = 0; i < MAX_INTERACTIONS; i++){
            user->interactions[i].itemId = 0;
            user->interactions[i].rating = 0;
        }
        return user;
    }
}

Movie* createMovie(int movieId, const char* movieTitle, int* genres){
    /*movieId'ye sahip movie yaratma ve yaratılıp yaratılamdığını kontrol etme*/
    Movie* movie = (Movie*)malloc(sizeof(Movie));
    if(movie == NULL){
        printf("movie %d icin hafizada gerekli yer ayrilamadi\n",movieId);
        return NULL;
    }
    /*movie eğer yaratıldıysa bilgilerini doldurma*/
    else{
        movie->movieId = movieId;

        /*movie'nin adını kaydetme(gerektiği kadar hafiza ayirarak)*/
        movie->movieTitle = malloc(MAX_MOVIE_TITLE_LENGTH * sizeof(char));
        strcpy(movie->movieTitle, movieTitle);
        int length = strlen(movie->movieTitle);
        movie->movieTitle = realloc(movie->movieTitle, length+1);
        movie->movieTitle[length] = '\0';

        /*movie'nin türlerini kaydetme*/
        for(int i = 0; i < MAX_GENRES; i++){
            movie->genres[i] = genres[i];
        }

        /*movie'nin etkileşimde olduğu user'ları kaydemek için gerekli yeri ayırma
          (burda her movie için hafızada eşit miktarda(gereğinden fazla) yer 
          ayrılacak ama ilerde her movie için reallocation fonksiyonu ile gereken 
          miktarda hafıza tahsis edilecektir.)*/
        movie->interactionCount = 0;
        movie->interactions = createInteraction();
        for(int i = 0; i < MAX_INTERACTIONS; i++){
            movie->interactions[i].itemId = 0;
            movie->interactions[i].rating = 0;
        }
        return movie;
    }
}

User* getUser(int userId, User** users){
    if(users[userId] == NULL){
        return NULL;
    }
    else{
        return users[userId];
    }
}

Movie* getMovie(int movieId, Movie** movies){
    if(movies[movieId] == NULL){
        return NULL;
    }
    else{
        return movies[movieId];
    }
}

/*-----ML-100K ADLI KLASÖRDEN VERİLE OKUMA İLE İLGİLİ FONKSİYONLAR-----*/

void loadMovieData(const char* filename, Movie** movies){
    FILE* file = fopen(filename, "r");
    /*Dosyanın açılıp açılmadığının kontrolü*/
    if(file == NULL){
        printf("Hata!!!Dosya acilamadi!\n");
        exit(1);
    }

    /*Dosya açıksa gerekli bilgileri dosyadan okuma ve kaydetme*/
    else{
        int movieId = 0;
        int genres[MAX_GENRES];
        char movieTitle[MAX_MOVIE_TITLE_LENGTH];
        char line[256];

        while(fgets(line, sizeof(line), file) != NULL){
            /*strtok fonksiyonu; stringi, parametre olarak verilen 
            karaktere göre böler ve ilk elemanın adresini döndürür.*/
            char *token = strtok(line, "|");

            //line'ın ilk elemanı
            if (token != NULL){
                movieId = atoi(token);
            }
            //line'ın ikinci elemanı
            token = strtok(NULL, "|");//token pointer'ını sıradaki elemanı gösterecek şekilde ayarlar
            if (token != NULL){
                strcpy(movieTitle,token);
            }
            token = strtok(NULL, "|");//filmin yayınlanma tarihi(gereksiz)
            token = strtok(NULL, "|");//filmin IMDB internet sitesi(gereksiz)
            /*Filmin sahip olduğu türlerin kaydı(line'ın beşinci elemanından itibaren)*/
            token = strtok(NULL, "|");
            for(int i = 0; i < MAX_GENRES && token != NULL; i++){
                genres[i] = atoi(token);
                token = strtok(NULL, "|");
            }

            /*Movie'yi yaratma*/
            if(movieId < MAX_MOVIES){
                Movie* movie = createMovie(movieId,movieTitle,genres);
                movies[movieId] = movie;
            }

            /*Maksimim movie sayısı aşıldıysa fonksiyonu durdurma*/
            else{
                printf("movieId %d maksimum film sayisini(%d) asiyor\n",movieId,MAX_MOVIES);
                break;
            }
        }
    }
    //Dosyayı kapatma
    fclose(file);
}

void loadUserData(const char* filename, User** users){
    /*Dosyanın açılıp açılmadığını kontrol etme*/
    FILE* file = fopen(filename,"r");
    if(file == NULL){
        printf("Hata!!!Dosya acilamadi!\n");
        exit(1);
    }

    /*Dosya açıksa gerekli bilgileri dosyadan okuma ve kaydetme*/
    else{
        int userId = 0;
        char line[40];

        while(fgets(line, sizeof(line), file) != NULL){

            /*strtok fonksiyonu; stringi, parametre olarak verilen 
            karaktere göre böler ve ilk elemanın adresini döndürür.*/
            char *token = strtok(line, "|");

            //line'ın ilk elemanı userId
            if (token != NULL){
                userId = atoi(token);
            }
            /*User'ı yaratma*/
            if(userId < MAX_USERS){
                User *user = createUser(userId);
                users[userId] = user;
            }
            /*Maximum user sayısı aşıldıysa fonksiyonu durdurma*/
            else{
                printf("userId %d maksimum user sayisini(%d) asti\n",userId,MAX_USERS);
                break;
            }
        }
    }
    //Dosyayı kapatma
    fclose(file);
}

void loadInteractionData(const char* filename, User** users, Movie** movies){
    /*Dosyanın açılıp açılmadığını kontrol etme*/
    FILE* file = fopen(filename,"r");
    if(file == NULL){
        printf("Hata!!!Dosya acilamadi!\n");
        exit(1);
    }

    /*Dosya açıksa gerekli bilgileri dosyadan okuma ve kaydetme*/
    else{
        int userId = 0;
        int movieId = 0;
        int rating = 0;
        int timestamp = 0;//kaydedilmeyecek bir veri

        while(fscanf(file, "%d %d %d %d",&userId, &movieId, &rating, &timestamp) == 4){
            User* user = getUser(userId, users);
            Movie* movie = getMovie(movieId, movies);

            /*user'ın var olup olamdığını kontrol etme*/
            if(user == NULL){
                printf("HATA!!!Bu ID'ye sahip user bulunmamaktadir\n");
                exit(1);
            }
            /*user eğer varsa user'ın izlediği filmleri ve verdiği puanları tutan tabloyu doldurma*/
            else{
                user->interactions[user->interactionCount].itemId = movieId;
                user->interactions[user->interactionCount].rating = rating;
                user->interactionCount++;
            }

            /*movie'nin var olup olmadığını kontrol etme*/
            if(movie == NULL){
                printf("HATA!!!Bu ID'ye sahip film bulunmamaktadir\n");
                exit(1);
            }
            /*movie eğer varsa movie'yi izleyen user ve verdikleri puanları tutan tabloyu doldurma*/
            else{
                movie->interactions[movie->interactionCount].itemId = userId;
                movie->interactions[movie->interactionCount].rating = rating;
                movie->interactionCount++;
            }
        }
    }
    //Dosyayı kapama
    fclose(file);
}

void reallocation(User** users, Movie** movies){
    /*user'lar için ayrılan fazla yeri iade etme*/
    for(int i = 1; i < MAX_USERS; i++){
        User* user = users[i];
        Interaction* temp = user->interactions;
        temp = (Interaction*)realloc(user->interactions, (user->interactionCount) * sizeof(Interaction));
        user->interactions = temp;
    }
    /*movie'ler için ayrılan fazla yeri iade etme*/
    for(int i = 1; i < MAX_MOVIES; i++){
        Movie* movie = movies[i];
        Interaction* temp = movie->interactions;
        temp = (Interaction*)realloc(movie->interactions, (movie->interactionCount) * sizeof(Interaction));
        movie->interactions = temp;
    }
}

/*-----BİRİNCİ ALGORİTMA-----*/

int createRandomNumber(){
    int randomMovieId = 0;
    /*0'dan farklı 1 ile 1682 arasinda rastgele sayi üret*/
    while(1){
        randomMovieId = rand() % MAX_MOVIES;
        if(randomMovieId != 0){
            break;
        }
    }
    return randomMovieId;
}

int unwatchedMovieId(User* user){
    /*user'ın var olup olmadığını kontrol etme*/
    if(user == NULL){
        printf("Hata!!!Böyle bir user bulunamamaktadir\n");
        return -1;
    }
    
    int randomMovieId = createRandomNumber();
    /*User filmi izlediyse fonksiyonu bir daha çağırma işlemi*/
    for(int j = 0; j < user->interactionCount; j++){
        if(randomMovieId == user->interactions[j].itemId){
            randomMovieId = unwatchedMovieId(user);
        }
    }
    return randomMovieId;
    
}

void suggestRandomMovies(User* user, Movie** movies, int suggestionNumber){
    /*user'ın var olup olmadığını kontrol etme*/
    if(user == NULL){
        printf("Hata!!!Böyle bir user bulunamamaktadir\n");
        return;
    }
    int count = 0;
    for(int i = 1; i <= suggestionNumber; i++){
        int randomMovieId = unwatchedMovieId(user);
        if(movies[randomMovieId] != NULL){
            count++;
            printf("%d. oneri: movieId: %d movieTitle: %s\n",i,randomMovieId , movies[randomMovieId]->movieTitle);
        }
        if(count == suggestionNumber){
            break;
        }
    }
}

/*-----İKİNCİ ALGORİTMA-----*/

int isUserWatchedThis(User* user, int movieId){
    /*user'ın var olup olmadığını kontrol etme*/
    if(user == NULL){
        printf("Hata!!!Böyle bir user bulunamamaktadir\n");
        return -1;
    }
    for(int i = 0; i < user->interactionCount; i++){
        if(user->interactions[i].itemId == movieId){
            return 1;//Kullanıcı filmi izlediyse 1 döndürür
        }
    }
    return 0;//Kullanıcı filmi izlemediyse 0 döndürür
}

void suggestTheMostWatchedMovies(User* user, Movie** movies, int suggestionNumber){
    /*user'ın var olup olmadığını kontrol etme*/
    if(user == NULL){
        printf("Hata!!!Böyle bir user bulunamamaktadir\n");
        return;
    }
    int count = 0;
    for(int i = 1; i < MAX_MOVIES; i++){
        if(movies[i]->interactionCount >= 300){
            int isWatched = isUserWatchedThis(user, i);
            if(isWatched == 0){
                count++;
                printf("%d. oneri: movieId: %d movieTitle: %s    interactionCount: %d\n",count, movies[i]->movieId ,movies[i]->movieTitle, movies[i]->interactionCount);
            }
            if(count == suggestionNumber){
                break;
            }
        }
    }
}

/*-----ÜÇÜNCÜ ALGORİTMA-----*/

int findUser(User* user, Movie** movies){
    /*user'ın var olup olmadığını kontrol etme*/
    if(user == NULL){
        printf("Hata!!!Böyle bir user bulunamamaktadir\n");
        return - 1;
    }
    int tablo[MAX_USERS] = {0};//user'ın diğer user'larla ortak izlediği film sayısını tutar
    
    /*user'ın her user'la kaç tane ortak film izlediğini tablo'ya kaydeder.*/
    for(int i = 0; i < user->interactionCount; i++){
        int movieId = user->interactions[i].itemId;
        for(int j = 0; j < movies[movieId]->interactionCount; j++){
            int userId = movies[movieId]->interactions[j].itemId;
            if(userId != user->userId){
                tablo[userId]++;
            }
        }
    }
    /*User'in en çok aynı film izlediği kullanıcının ID'sini bulur*/
    int max = tablo[1];
    int max_index = 1;
    for(int i = 2; i < MAX_USERS; i++){
        if(max < tablo[i]){
            max = tablo[i];
            max_index = i;
        }
    }
    printf("En cok ortak film izledigi kullanicinin ID'si: %d\n",max_index);
    return max_index;
}

void suggestMovieWithAnotherUser(User* user, User** users, Movie** movies, int suggestionNumber){
    /*user'ın var olup olmadığını kontrol etme*/
    if(user == NULL){
        printf("Hata!!!Böyle bir user bulunamamaktadir\n");
        return;
    }
    int userId = findUser(user, movies);//en fazla sayıda ayni film izlediği kullanıcı
    int count = 0;//sayaç. Kaç tane film önerildiğini tutar
    for(int i = 0; i < users[userId]->interactionCount; i++){
        if(users[userId]->interactions[i].rating == 5){
            int movieId = users[userId]->interactions[i].itemId;
            int watched = 0;//kullanıcı filmi izlememişse 0 olarak kalacak
            /*Kullanıcının filmi izleyip izlemediğini kontrol etme kısmı*/
            for(int j = 0; j < user->interactionCount; j++){
                if(user->interactions[j].itemId == movieId){
                    watched = 1;//kullanıcı filmi daha önce izlediyse kontrol degiskenine 1 değeri atanıyor
                    break;
                }
            }
            //Kullanıcı filmi izlemediyse filmi kullanıcıya önerme kısmı
            if(watched == 0){
                count++;
                printf("%d. oneri: movieId: %d movieTitle: %s\n",count, movieId, movies[movieId]->movieTitle);
            }
            if(count == suggestionNumber){
                break;
            }
        }
    }
}

/*-----DÖRDÜNCÜ ALGORİTMA-----*/

void suggestMovieWithTheLowestWeightedPathLength(User* user, User** users, Movie** movies, int suggestionNumber){
    /*user'ın var olup olmadığını kontrol etme*/
    if(user == NULL){
        printf("Hata!!!Böyle bir user bulunamamaktadir\n");
        return;
    }
    int tablo[MAX_MOVIES];
    /*Kullanıcının tüm movie'lere uzaklığı default şekilde 20 olarak belirlenir*/
    for(int i = 0; i < MAX_MOVIES; i++){
        tablo[i] = 20;
    }
    int length1 = 0;
    int length2 = 0;
    int length3 = 0;
    for(int i = 0; i < user->interactionCount; i++){//user'ın izlediği filmler tek tek dolaşılıyor
        Movie* movie = movies[user->interactions[i].itemId];//user'ın izlediği movie
        length1 = user->interactions[i].rating;//user'ın movie'ye olan ağırlıklı uzaklığı length1'e kaydediliyor
        for(int j = 0; j < movie->interactionCount; j++){//user'ın izlediği filmleri izleyen user'lar tek tek dolaşılıyor
            User* userVar = users[movie->interactions[j].itemId];//movie'yi izleyen userVar
            length2 = length1 + movie->interactions[j].rating;//userVar'ın user'a olan ağırlıklı uzaklığı length2'ye kaydediliyor
            for(int k = 0; k < userVar->interactionCount; k++){//userVar'ların izlediği filmler tek tek dolaşılıyor
                Movie* movieVar = movies[userVar->interactions[k].itemId];//userVar'ın izlediği movieVar
                length3 = length2 + userVar->interactions[k].rating;//movieVar'ın user'a olan uzaklığı lenth3'e kaydediliyor
                tablo[movieVar->movieId] = length3;//length3 tabloda movieVar'ın ID'sine kaydediliyor
            }
        }
    }
    /*Ağırlıklı uzaklığı en düşük olan film bulunuyor*/
    int min_ind = 0;
    int min_path_long = 20;
    for(int i = 1; i < MAX_MOVIES; i++){
        if(tablo[i] < min_path_long){
            min_ind = i;
            min_path_long = tablo[i];
        }
    }
    int count = 0;
    for(int i = 1; i < MAX_MOVIES; i++){
        if(tablo[i] == min_path_long){
            if(isUserWatchedThis(user, i) == 0){//user filmi izlemediyse önerme işlemi
                count++;
                printf("%d. oneri: movieId: %d movieTitle: %s\n", count, movies[i]->movieId, movies[i]->movieTitle);
            }
            if(count == suggestionNumber){
                break;
            }
        }
    }
}

/*-----BEŞİNCİ ALGORİTMA-----*/

void suggestMovieWithTheMostWatchedGenre(User* user, Movie** movies, int suggestionNumber){
    /*user'ın var olup olmadığını kontrol etme*/
    if(user == NULL){
        printf("Hata!!!Böyle bir user bulunamamaktadir\n");
        return;
    }
    int tablo[MAX_GENRES] = {0};
    /*Tablo'yu doldurma*/
    for(int i = 0; i < user->interactionCount; i++){
        if(user->interactions[i].rating >= 4){//user'ın 4 veya 5 puan verdiği filmleri tara
            Movie* movie = movies[user->interactions[i].itemId];
            for(int j = 0; j < MAX_GENRES; j++){
                /*movie->genres tablosu 19 kutusu olan bir int tablosudur. Film bir türü içeriyorsa
                  o kutunun değeri 1'dir, içermiyorsa 0'dır.*/
                tablo[j] = tablo[j] + movie->genres[j];//tabloyu doldur
            }
        }
    }
    /*user'ın en çok sevdiği türü bulma işlemi*/
    int max_ind = 0;
    int max = 0;
    for(int i = 0; i < MAX_GENRES; i++){
        if(max < tablo[i]){
            max_ind = i;
            max = tablo[i];
        }
    }
    int count = 0;
    /*user'a film önerme işlemi*/
    for(int i = 1; i < MAX_MOVIES; i++){
        if(isUserWatchedThis(user, i) == 0){
            Movie* movie = movies[i];
            if(movie->genres[max_ind] == 1 && movie->interactionCount >= 150){
                count++;
                printf("%d. oneri: movieID: %d movieTitle: %s\n",count, i, movie->movieTitle);
            }
            if(count == suggestionNumber){
                break;
            }
        }
    }
    /*user'ın beğendiği film yoksa rastgele film önerme işlemi*/
    if(count == 0){
        printf("Kullanicinin begendiği film bulunmamaktadir. Bu yuzden rastgele film onerilecektir\n");
        suggestRandomMovies(user, movies, suggestionNumber);
    }
}

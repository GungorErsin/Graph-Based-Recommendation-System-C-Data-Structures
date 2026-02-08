#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "proje.h"

int main(){
    srand(time(NULL));
    /*Kullanıcılara ve filmlere erişim için tablo tanımlama*/
    User* users[MAX_USERS];
    Movie* movies[MAX_MOVIES];
    /*Veri setinden verileri okuma ve tablolara kaydetme*/
    char movieFile[10] = "u.item";
    loadMovieData(movieFile, movies);
    char userFile[10] = "u.user";
    loadUserData(userFile, users);
    char testFile[10] = "u.data";
    loadInteractionData(testFile, users, movies);
    /*Gereksiz hafıza kullanımını düzeltme*/
    reallocation(users, movies);

    /*Program*/
    int secim = 1;
    while(1){
        printf("Yapmak istediginiz islemi tuslayiniz\n");
        printf("0-)Programdan cikmak icin 0'i tuslayiniz\n");
        printf("1-)Kullaniciya, rastgele film onerisi icin 1'i tuslayiniz\n");
        printf("2-)Kullaniciya, en cok izlenen filmler arasindan film onerisi icin 2'yi tuslayiniz\n");
        printf("3-)Kullaniciya, en cok sayida ayni film izledigi kullanicinin en cok begendigi filmler arasindan film onerisi icin 3'u tuslayiniz\n");
        printf("4-)Kullaniciya, agirlikli yol uzunlugunun en az oldugu filmler arasindan film onerisi icin 4'u tuslayiniz\n");
        printf("5-)Kullaniciya, en cok begendigi ture sahip izlemedigi filmler arasindan film onerisi icin 5'i tuslayiniz\n");
        scanf("%d",&secim);
        if(secim == 0){
            printf("Program sonlandiriliyor...\n");
            return 0;
        }
        else if(secim == 1){
            int suggestionNumber = 0;
            int userId = 0;
            while(1){
                printf("Film onermek istediginiz kullanicinin userId'sini giriniz.(1 ile 943 arasinda olmak zorunda)\n");
                scanf("%d",&userId);
                if(userId < 1 || userId > 943){
                    printf("Lutfen gecerli bir userId giriniz\n");
                }
                else{
                    break;
                }
            }
            printf("Kac tane film onermek istiyorsunuz?\n");
            scanf("%d",&suggestionNumber);
            printf("\n\n-----ONERILER-----\n");
            suggestRandomMovies(users[userId], movies, suggestionNumber);
            printf("\nAna menuye donuluyor...\n\n");
        }
        else if(secim == 2){
            int suggestionNumber = 0;
            int userId = 0;
            while(1){
                printf("Film onermek istediginiz kullanicinin userId'sini giriniz.(1 ile 943 arasinda olmak zorunda)\n");
                scanf("%d",&userId);
                if(userId < 1 || userId > 943){
                    printf("Lutfen gecerli bir userId giriniz\n");
                }
                else{
                    break;
                }
            }
            printf("Kac tane film onermek istiyorsunuz?\n");
            scanf("%d",&suggestionNumber);
            printf("\n\n-----ONERILER-----\n");
            suggestTheMostWatchedMovies(users[userId], movies, suggestionNumber);
            printf("\nAna menuye donuluyor...\n\n");
        }
        else if(secim == 3){
            int suggestionNumber = 0;
            int userId = 0;
            while(1){
                printf("Film onermek istediginiz kullanicinin userId'sini giriniz.(1 ile 943 arasinda olmak zorunda)\n");
                scanf("%d",&userId);
                if(userId < 1 || userId > 943){
                    printf("Lutfen gecerli bir userId giriniz\n");
                }
                else{
                    break;
                }
            }
            printf("Kac tane film onermek istiyorsunuz?\n");
            scanf("%d",&suggestionNumber);
            printf("\n\n-----ONERILER-----\n");
            suggestMovieWithAnotherUser(users[userId], users, movies, suggestionNumber);
            printf("\nAna menuye donuluyor...\n\n");
        }
        else if(secim == 4){
            int suggestionNumber = 0;
            int userId = 0;
            while(1){
                printf("Film onermek istediginiz kullanicinin userId'sini giriniz.(1 ile 943 arasinda olmak zorunda)\n");
                scanf("%d",&userId);
                if(userId < 1 || userId > 943){
                    printf("Lutfen gecerli bir userId giriniz\n");
                }
                else{
                    break;
                }
            }
            printf("Kac tane film onermek istiyorsunuz?\n");
            scanf("%d",&suggestionNumber);
            printf("\n\n-----ONERILER-----\n");
            suggestMovieWithTheLowestWeightedPathLength(users[userId], users, movies, suggestionNumber);
            printf("\nAna menuye donuluyor...\n\n");
        }
        else if(secim == 5){
            int suggestionNumber = 0;
            int userId = 0;
            while(1){
                printf("Film onermek istediginiz kullanicinin userId'sini giriniz.(1 ile 943 arasinda olmak zorunda)\n");
                scanf("%d",&userId);
                if(userId < 1 || userId > 943){
                    printf("Lutfen gecerli bir userId giriniz\n");
                }
                else{
                    break;
                }
            }
            printf("Kac tane film onermek istiyorsunuz?\n");
            scanf("%d",&suggestionNumber);
            printf("\n\n-----ONERILER-----\n");
            suggestMovieWithTheMostWatchedGenre(users[userId], movies, suggestionNumber);
            printf("\nAna menuye donuluyor...\n\n");
        }
        else{
            printf("Lutfen gecerli bir sayi tuslayiniz\n\n");
        }
    }
    
    return 0;    
}

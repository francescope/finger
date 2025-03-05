#ifndef HW1_H
#define HW1_H

#include <stddef.h> // per definizioni comuni come size_t e NULL
#include <stdio.h>  // per funzioni di input/output standard come printf
#include <fcntl.h>  // per la funzione open
#include <unistd.h> // per funzioni di sistema come close, read, write
#include <string.h> // per funzioni di manipolazione delle stringhe come strdup, strcpy, strncpy
#include <stdlib.h> // per funzioni di allocazione dinamica (malloc, free)
#include <time.h>   // per funzioni di manipolazione del tempo standard
#include <pwd.h>    // per la struttura passwd e le funzioni getpwent, endpwent
#include <utmpx.h>  // per la struttura utmpx e le funzioni getutxent, endutxent


/* Struttura che mi consente di creare una linked list di Info_utente, un pacchetto
 contenente tutte le possibili informazioni di cui finger potrebbe aver bisogno */
struct Info_utente 
{
    char *login;        //login name 
    char *name;         //fullname 
    char *tty_on;       //indica il terminale specifico (dispositivo tty) su cui l'utente è connesso
    char *tty_from;     //indica l'host remoto o l'indirizzo IP da cui l'utente si è connesso.
    long long int idle; //idle time
    time_t login_time;  //login time
    char *office;       //numero di stanza 
    char *office_phone; //numero di telefono aziendale
    char *home_phone;   //numero di telefono personale
    char *directory;    //percorso della directory home dell'utente
    char *shell;        //percorso del programma shell (interfaccia a riga di comando) predefinito per l'utente
    struct Info_utente *next; //puntatore al prossimo nodo
};

/* Struttura che mi consente di creare una linked list di utenti specificati dal loro nome*/
struct User {
    char *username; 
    struct User *next;
};

struct User *head; //testa della lista collegata
struct Info_utente *info_head; //testa della lista collegata

void addUserToList(const char *username);
void addInfoToList(struct Info_utente *info);
int estrai_utenti_e_opzioni(char** input, char** opz, int* opz_m, int* opz_p);
void info_utenti_connessi();
char* formatta_telefono(const char* tel);
void spacchetta_gecos(struct Info_utente *info, char* gecos);
char* estract_fullname(const char* gecos);
void set_passwd_info(struct Info_utente *info, struct passwd *pw);
void set_utmpx_info(struct Info_utente *info, struct utmpx *ut);
int estract_info(char* utente, char** opz, int* opz_m);
void finger_flat(struct Info_utente *info);
void read_file_char_by_char(const char *name_file, const char *name_user);
void finger_detailed(struct Info_utente *info, char** opz, int* opz_p);

#endif // HW1_H
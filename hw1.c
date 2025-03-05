#include "hw1.h"

struct User *head = NULL; //testa della lista collegata
struct Info_utente *info_head = NULL; //testa della lista collegata


/* Questa funzione di occupa di aggiungere l'utente specificato nella coda della lista*/
void addUserToList(const char *username) 
{
    struct User *newUser = malloc(sizeof(struct User)); //allocazione dinamica della struct User per il nuovo utente
    newUser->username = strdup(username); //imposta il puntatore username del nuovo utente
    newUser->next = NULL;                 //imposta il puntatore next del nuovo utente

    //se la lista è vuota, il nuovo utente diventa la testa della lista
    if (head == NULL) {
        head = newUser;
    }
    //altrimenti, scorri la lista fino all'ultimo elemento
    else {
        struct User *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newUser; //aggiungi il nuovo utente alla fine della lista
    }
}

/* Questa funzione di occupa di aggiungere il "pacchetto" di info sul utente specificato nella coda della lista*/
void addInfoToList(struct Info_utente *info) 
{
    struct Info_utente *new_info = malloc(sizeof(struct Info_utente)); //allocazione dinamica della struct Info_utente per il nuovo utente
 
    // Copia profonda della struttura Info_utente
    memcpy(new_info, info, sizeof(struct Info_utente));

    new_info->login = strdup(info->login);
    new_info->name = strdup(info->name);
    new_info->tty_on = info->tty_on ? strdup(info->tty_on) : NULL;
    new_info->tty_from = info->tty_from ? strdup(info->tty_from) : NULL;
    new_info->office = info->office ? strdup(info->office) : NULL;
    new_info->office_phone = info->office_phone ? strdup(info->office_phone) : NULL;
    new_info->home_phone = info->home_phone ? strdup(info->home_phone) : NULL;
    new_info->directory = strdup(info->directory);
    new_info->shell = strdup(info->shell);
    new_info->next = NULL; //imposta il puntatore next del nuovo pacchetto

    //se la lista è vuota, il nuovo pacchetto di informazioni diventa la testa della lista
    if (info_head == NULL) {
        info_head = new_info;
    } 
    //altrimenti, scorri la lista fino all'ultimo elemento
    else {
        struct Info_utente *current = info_head;
        //scorre la linked list per posizionare l'elemento sulla coda
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_info;
    }
}


/* Questa funzione si occupa di estrarre gli utenti dal input e inserirli in una lista, inoltre se presente si salva l'opzione [-lsmp] */
int estrai_utenti_e_opzioni(char** input, char** opz, int* opz_m, int* opz_p) 
{
    int are_users_specified = 0;

    //se non viene usato il comando finger restituisco con intero che simboleggia un errore
    if (strcmp(input[1], "finger") != 0) {
        printf("Command '%s' not found, did you mean:\n command 'finger' from deb finger (0.17-17)\n command 'fingerw' from deb libwww-finger-perl (0.105-2)\nTry: sudo apt install <deb name>\n", input[1]);
        return -1;
    }

    int i = 2; //argv[0] è il percorso del file //argv[1] = "finger"
    while (input[i] != NULL)
    {
        if (strchr(input[i], '-') != NULL)
        {
            //analizzo l'opzione. (comincio da 1 perchè input[i][j] = "-")
            for (int j = 1; input[i][j] != '\0'; j++) 
            {           
                //che schema di output devo usare?
                if ((input[i][j] == 's') || (input[i][j] == 'l')) 
                {
                    if (strcmp(*opz, "-l") != 0)  //-l ha priorità maggiore rispetto a -s quindi se specificati entrambi opz = -l
                    { 
                        char opzione[3];
                        sprintf(opzione, "-%c", input[i][j]);
                        *opz = strdup(opzione);
                    }
                }
                //flag -m
                else if (input[i][j] == 'm') {
                    *opz_m = 1;
                }
                //flag -p
                else if (input[i][j] == 'p') {
                    *opz_p = 1;
                }
                //opzione che non esiste
                else {
                    printf("finger: invalid option -- '-%c'\nusage: finger [-lmps] [login ...]\n", input[i][j]);
                    return -1;
                }
            }
        }
        //non è un opzione ma un nome utente
        else {
            addUserToList(input[i]);
            are_users_specified = 1;
        }
        i++;
    }
    return are_users_specified;
}


/* Filtra tutti gli utenti attivi e li mette nella lista */
void info_utenti_connessi() 
{ 
    int num_utenti = 0;
    struct utmpx *utente;
    while ((utente = getutxent()) != NULL) {
            if (utente->ut_type == USER_PROCESS) { //se l'utente è attivo
                addUserToList(utente->ut_user);
                num_utenti++;
            }
    }
}

/* Formatta i numeri di telefono di 11,10,7,5,4 caratteri mentre gli altri no*/
char* formatta_telefono(const char* tel) 
{
    int n = strlen(tel);
    char* telefono = malloc(16 * sizeof(char)); // Allocazione dinamica per il risultato, 11 max digits + [+,-,x]separatori (max 4) + '\0
    
    //+N-NNN-NNN-NNNN
    if (n == 11) { 
        sprintf(telefono, "+%c-%c%c%c-%c%c%c-%c%c%c%c", tel[0], tel[1], tel[2], tel[3], tel[4], tel[5], tel[6], tel[7], tel[8], tel[9], tel[10]); 
    } 
    //NNN-NNN-NNNN
    else if (n == 10) { 
        sprintf(telefono, "%c%c%c-%c%c%c-%c%c%c%c", tel[0], tel[1], tel[2], tel[3], tel[4], tel[5], tel[6], tel[7], tel[8], tel[9]);
    } 
    //NNN-NNNN
    else if (n == 7) {
        sprintf(telefono, "%c%c%c-%c%c%c%c", tel[0], tel[1], tel[2], tel[3], tel[4], tel[5], tel[6]);
    } 
    //xN-NNNN
    else if (n == 5) {
        sprintf(telefono, "x%c-%c%c%c%c", tel[0], tel[1], tel[2], tel[3], tel[4]);
    } 
    //xNNNN
    else if (n == 4) {
        sprintf(telefono, "x%c%c%c%c", tel[0], tel[1], tel[2], tel[3]);
    } 
    // copia il numero senza formattazione se non corrisponde a nessuna delle lunghezze attese
    else {
        strcpy(telefono, tel); 
    }
    return telefono;
}

/* Siccome il campo pw_gecos contiene più informazioni diverse in esso, le estraggo. */
void spacchetta_gecos(struct Info_utente *info, char* gecos) 
{
    char *fullname = NULL;
    char *office = NULL;
    char *office_phone = NULL;
    char *home_phone = NULL;

    char* fields[] = {fullname, office, office_phone, home_phone};

    int i = 0; //indice nella stringa completa gecos
    int j = 0; //numero campo in gecos: 0 = fullname, 1 = office, 2 = office_phone, 3 = home_phone
    int c = 0; //indice nel campo specifico di gecos

    while (gecos[i] != '\0' && j < 4) //se finisco di leggere gecos o riempio tutti i campi che mi servono
    {
        if (gecos[i] == ',') //"," è il carattere separatore tra i campi
        {
            fields[j] = (char*) realloc(fields[j], (c + 1) * sizeof(char)); //aggiungi spazio per il terminatore null
            fields[j][c] = '\0'; //mi assicuro che la stringa del campo corrente abbia terminazione null
            j++;
            c = 0; //resetta l'indice di campo per il prossimo campo
        }
        else 
        {
            fields[j] = (char*) realloc(fields[j], (c + 1) * sizeof(char)); //aggiungi spazio per il carattere corrente
            fields[j][c] = gecos[i];
            c++;
        }
        i++;
    }

    // Aggiunge il terminatore null per l'ultimo campo, se esiste
    if (j < 4) {
        fields[j] = (char*) realloc(fields[j], (c + 1) * sizeof(char));
        fields[j][c] = '\0';
    }

    info->name = strdup(fields[0]);
    info->office = strdup(fields[1]);
    info->office_phone = (strlen(fields[2]) > 0) ? strdup(formatta_telefono(fields[2])) : strdup(fields[2]);
    info->home_phone = (strlen(fields[3]) > 0) ? strdup(formatta_telefono(fields[3])) : strdup(fields[3]);
}

/* Estrae solo il fullname dal campo gecos poichè, anch'esso mi serve per cercare le corrispondenze in passwd e utmpx*/
char* estract_fullname(const char* gecos) 
{
    const char *comma_pos = strchr(gecos, ','); //trova la posizione della virgola
    size_t len = comma_pos - gecos; //calcola il numero di caratteri da copiare

    //allocare memoria per fullname e assicurarsi di non copiare più della dimensione del buffer
    char *fullname = (char*)malloc(len + 1);
    if (fullname) {
        strncpy(fullname, gecos, len); //copia i caratteri fino alla virgola
        fullname[len] = '\0'; //aggiungi il terminatore di stringa 
    }
    else {
        fullname = strdup(gecos); //se il campo gecos rappresenta semplicemente fullname e non anche altro
    }
    return fullname;
}

/* Imposta i campi di info_utente relativi al file etc/passwd */
void set_passwd_info(struct Info_utente *info, struct passwd *pw) 
{ 
    info->login = strdup(pw->pw_name); 
    info->directory = strdup(pw->pw_dir);
    info->shell = strdup(pw->pw_shell);
    spacchetta_gecos(info, pw->pw_gecos); //funzione che estrae e salva le informazioni di pw_gecos
}

/* Imposta i campi di info_utente relativi al file /var/run/utmp */
void set_utmpx_info(struct Info_utente *info, struct utmpx *ut)
{
    //buffers temporanei per memorizzare i valori dei campi tty_on e tty_from
    char tty_on_buffer[__UT_LINESIZE + 1];
    char tty_from_buffer[__UT_HOSTSIZE + 1];

    strncpy(tty_on_buffer, ut->ut_line, __UT_LINESIZE); //copia il campo ut_line da utmpx a tty_on_buffer
    tty_on_buffer[__UT_LINESIZE] = '\0'; //aggiungi un terminatore nullo
    info->tty_on = strdup(tty_on_buffer); //assegna una copia del tty_on_buffer a info->tty_on usando strdup per l'allocazione dinamica

    strncpy(tty_from_buffer, ut->ut_host, __UT_HOSTSIZE); //copia il campo ut_host da utmpx a tty_from_buffer
    tty_from_buffer[__UT_HOSTSIZE] = '\0'; //aggiungi un terminatore nullo
    info->tty_from = strdup(tty_from_buffer); //assegna una copia del tty_from_buffer a info->tty_from usando strdup per l'allocazione dinamica

    info->login_time = ut->ut_tv.tv_sec; 
    time_t current_time = time(NULL);
    info->idle = current_time - ut->ut_tv.tv_sec; //calcola il tempo di inattività (idle) sottraendo il tempo di login dal tempo attuale
}


/* Coordina le chiamate a funzioni più specifiche per estrarre le informazioni dai file passwd e utmpx relative ad un utente
e le salva in una variabile Info_utente che aggiunge ad una specifica lista*/
int estract_info(char* utente, char** opz, int* opz_m) 
{    
    struct passwd *pw; //puntatore alla struttura struct passwd
    struct utmpx *ut;  //puntatore alla struttura struct utmpx

    int is_user_real = 0; //flag per verificare se l'utente esiste
   
    setpwent();    //apre il file passwd
    setutxent();   //apre il file utmpx

    while ((pw = getpwent()) != NULL) //itera sulle righe del file passwd
    {

        //estraggo il fullname poichè posso fare match anche su esso
        char* fullname = NULL;
        if (*opz_m) { //l'opzione -m evita il metching con il fullname
            fullname = "";
        }
        else {
            fullname = estract_fullname(pw->pw_gecos); 
        }

        //controlla se il nome utente o il fullname corrispondono all'utente cercato
        if ((strcmp(fullname, utente) == 0) || (strcmp(pw->pw_name, utente) == 0)) 
        {
            struct Info_utente *info = malloc(sizeof(struct Info_utente)); //allocazione dinamica per info_utente

            set_passwd_info(info, pw); //imposta i campi di info_utente che leggo del file passwd
            is_user_real = 1; //l'utente esiste

            int is_active = 0; //flag per verificare se l'utente è attivo
            while ((ut = getutxent()) != NULL) 
            {
                if ((strncmp(ut->ut_user, pw->pw_name, sizeof(ut->ut_user)) == 0) )
                {
                    set_utmpx_info(info, ut); //imposta i campi di info_utente che leggo del file utmpx
                    is_active = 1;
                }
            }
            //se l'utente non è attivo, resetta i campi tty_on e tty_from
            if (is_active == 0) {
                info->tty_on = NULL;
                info->tty_from = NULL;
            }
            addInfoToList(info);
        } 
    }
    endutxent(); //chiude il file utmpx
    endpwent();  //chiude il file passwd

    if (!is_user_real) 
        return -1; //errore "l'utente specificato non esiste". Lo faccio sapere ritornando -1
    return 0;
}


/* Stampa le informazioni necessarie al formato monoriga nella loro specifica formattazione*/
void finger_flat(struct Info_utente *info) 
{
    //se il formato del numero di telefono è illegale stampa ""
    int n = strlen(info->office_phone); //lunghezza nunero di telefono

    if (info->tty_on) //se l'utente è loggato sarà sicuramente su un terminale
    {
        //array di stringhe che rappresentano i nomi dei mesi
        const char *mesi[] = {"Jan", "Feb", "Mar", "Apr","May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

        //calcola il numero di ore, minuti e secondi di idle
        int hours = info->idle / 3600;
        int minutes = (info->idle % 3600) / 60;
        int seconds = info->idle % 60;

        //formatta il tempo di idle in base alla sua grandezza
        char output_idle[10];
        if (hours > 0) {
            sprintf(output_idle, "%02d:%02d", hours, minutes); // se sono trascorse più di 1 ora, formatta il tempo come HH:MM
        } 
        else if (hours == 0 && minutes == 0) {
            strcpy(output_idle, ""); //assegna una stringa vuota se parliamo solo di secondi
        }
        else if (hours == 0) {
            sprintf(output_idle, "%2d", minutes); //formatta il tempo come MM
        }
    
        //formatta il tempo di login
        char output_login_t[20];
        struct tm *tm_info = localtime(&info->login_time);
        sprintf(output_login_t, "%s %d %02d:%02d", mesi[tm_info->tm_mon], tm_info->tm_mday, tm_info->tm_hour, tm_info->tm_min);

        //riconosco un numero di telefono illegale se non corrisponde a determinate lunghezze
        if (n!=15 && n!=12 && n!= 8 && n!=7 && n!=5) {
            printf("%-10s %-10s %-8s %-8s %-15s (%s) %-15s\n", info->login, info->name, info->tty_on, output_idle, output_login_t, info->tty_from, "");
        }
        else {
            printf("%-10s %-10s %-8s %-8s %-15s (%s) %-15s\n", info->login, info->name, info->tty_on, output_idle, output_login_t, info->tty_from, info->office_phone);
        }
    }
    else 
    {
        if (n!=15 && n!=12 && n!= 8 && n!=7 && n!=5) {
            printf("%-10s %-10s %-8s %-8s %-15s %-10s %-15s\n", info->login, info->name, "*", "*", "No Logins", info->office, "");
        }
        else {
            printf("%-10s %-10s %-8s %-8s %-15s %-10s %-15s\n", info->login, info->name, "*", "*", "No Logins", info->office, info->office_phone);
        }
    }
}


/* Apre e stampa il contenuto del file specificato in input*/
void read_file_char_by_char(const char *name_file, const char *name_user) 
{
    char *home_dir = malloc(strlen("/home/") + strlen(name_user) + 1); //costruisci la directory home
    snprintf(home_dir, strlen("/home/") + strlen(name_user) + 1, "/home/%s", name_user); //formattazione della stringa

    // determina il percorso del file
    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/.%s", home_dir, name_file);

    //prova ad aprire il file
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        //l'unico caso in cui riferisco che il file non è presente è per il .plan
        if (strcmp(name_file, "plan") == 0) {
            printf("No Plan.\n");
        }
    }
    else {
        //stampa l'intestazione appropriata
        if (strcmp(name_file, "forward") == 0) {
            printf("Mail forwarded to ");
        }
        else if (strcmp(name_file, "pgpkey") == 0) {
            printf("PGP key: ");
        }
        else {
            printf("%s:\n", name_file);
        }

        //leggi e stampa il contenuto del file
        int ch;
        while ((ch = fgetc(file)) != EOF) { //stampo carattere per carattere 
                putchar(ch);
        }
        fclose(file);
    }
    free(home_dir);
}


/* Stampa le informazioni necessarie al formato su più righe nella loro specifica formattazione*/
void finger_detailed(struct Info_utente *info, char** opz, int* opz_p) 
{
    printf("Login: %-25s            Name: %-25s\n", info->login, info->name);
    printf("Directory: %-25s        Shell: %-25s\n", info->directory, info->shell);
    printf("Office: %s, %-25s       Home Phone: %-15s\n", info->office, info->office_phone , info->home_phone);
    
    //controlla se l'utente non si è mai loggato
    if (info->login_time == 0) {
        printf("Never logged in.\n");
    } 
    else {
        //formatta e stampa il tempo di login
        char login_time_str[256];
        struct tm *tm_info = localtime(&info->login_time);

        if (tm_info != NULL)
        {
            strftime(login_time_str, sizeof(login_time_str), "%a %b %d %H:%M (%Z)", localtime(&info->login_time));
            printf("On since %s on %s from %s\n", login_time_str, info->tty_on, info->tty_from);

            //calcola e stampa il tempo di inattività
            int hours = info->idle / 3600;
            int minutes = (info->idle % 3600) / 60;
            int seconds = info->idle % 60;

            if (hours == 0) {
                printf("    %2d minutes %2d seconds idle\n", minutes, seconds);
            }
            else if(hours == 1) {
                printf("    %2d hour %2d minutes idle\n", hours, minutes);
            }
            else {
                printf("    %2d hours %02d minutes idle\n", hours, minutes);
            }
        }
    }
    read_file_char_by_char("forward", info->login);
    printf("No mail.\n");

    //controlla l'opzione -p per determinare quali file leggere
    if (!*opz_p) {
        read_file_char_by_char("project", info->login);
        read_file_char_by_char("plan", info->login);
        read_file_char_by_char("pgpkey", info->login);
    }
    printf("\n");
}



int main(int argc, char* argv[]) {

    char *opz = "default"; //opzione comando inizializzata a default ovvero non specificata
    int opz_m = 0; //flag -m
    int opz_p = 0; //flag -p

    int are_users_specified = estrai_utenti_e_opzioni(argv, &opz, &opz_m, &opz_p);

    //se specifico un opzione che non esiste o un comando che non sia finger esco subito
    if (are_users_specified != -1) {

        /*lavoro con gli utenti connessi al sistema oppure con quelli specificati dall' utente chiamando finger. 
        In ogni caso alla fine di questo blocco avrò una lista di nomi di utenti su cui lavorare*/
        if (are_users_specified == 0) 
            info_utenti_connessi();

        struct User *current = head; //puntatore per scorrere la lista di utenti 
        struct Info_utente *current_info = NULL; //puntatore per scorrere la lista di info_utente

        // 1 = -s formato // 0 = -l formato
        int is_flat = ((strcmp(opz, "default") == 0 && are_users_specified == 0) || strcmp(opz, "-s") == 0);

        //stampa i campi (una sola volta se in formato -s)
        if (is_flat)
            printf("%-10s %-10s %-8s %-8s %-15s %-10s %-15s\n", "Login", "Name", "Tty", "Idle", "Login Time", "Office", "Office Phone");
            
        //scorro la lista di utenti
        while (current != NULL) 
        {
            int exists_user = estract_info(current->username, &opz, &opz_m); //raccolgo tutte le informazioni possibili su un utente e aggiunge il "pacchetto" alla lista
            
            //utente non trovato => non esiste
            if (exists_user ==  -1) {
                printf("finger: %s: no such user\n", current->username);
            }
            current = current->next; //passa al prossimo nodo
        }

        //scorro lista di info_utente
        current_info = info_head; 
        while (current_info != NULL)
        {
            if (is_flat) {
                finger_flat(current_info);
            }
            else {
                finger_detailed(current_info, &opz, &opz_p);
            }
            current_info = current_info->next; //passa al prossimo nodo
        }
    }
}
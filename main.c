#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define num_guest 8
#define MAX_BOREK 4
#define MAX_CAKE 2
#define MAX_DRINK 4
int borekconsumecount = 0;
int drinkconsumecount = 0;
 int borek_number = 30;
    int slices_of_cake = 15;
    int glasses_of_drinks_number = 30;
    int serve_borek;
    int serve_cake;
    int serve_drink;
   const int cake_tray_max = 5;
   const int drink_tray_max = 5;
    const int borek_tray_max = 5;
    int cake_tray = 0;
    int drink_tray = 0;
    int borek_tray = 0;
    int r;
    pthread_t thread_id;
    pthread_t thread_ids[8];
    int guest_ids[8];
    int g1cake,g1borek,g1drink;
    int randomizer = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void update_food(int borek, int cake, int drink);
typedef void *(*GuestFunction)(void *);
int foodPick(); 
typedef struct{
    int borek_consumed;
    int cake_consumed;
    int drink_consumed;
} GuestInfo;

GuestInfo guest_info[num_guest];

int foodPick()
{
    srand(time(NULL));
    return rand() % 3 + 1;
}

void *guestBehavior(void *arg)
{

    int guest_id = *(int *)arg;
    GuestInfo *info = &guest_info[guest_id];

    pthread_mutex_lock(&mutex);
    printf("borek count and drink count: %d %d\n", borekconsumecount,drinkconsumecount);
    printf("Guest %d is activated\n", guest_id);
    
    
    int randomei = foodPick();
    if(randomei == 1 && info->borek_consumed == MAX_BOREK){
        randomei++;
    }
    if(randomei == 2 && info->cake_consumed == MAX_CAKE){
        randomei++;
    }
    if(randomei == 3 && info->drink_consumed == MAX_DRINK  ){
        randomei = 1;
    }
    
    if (randomei == 1) {
        if (borek_number <= 0 && borek_tray == 0) {
            borek_number = 0;
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        else{
            if (info->borek_consumed == MAX_BOREK) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }   else {
            srand(time(NULL));
            serve_borek = rand() % 5;

            if(info->borek_consumed + serve_borek > MAX_BOREK){
                serve_borek = MAX_BOREK - info->borek_consumed;
            }

            if(serve_borek > borek_tray){
                serve_borek = borek_tray;
            }
            printf("Guest %d: Serving %d boreks\n", guest_id, serve_borek);

            

            info->borek_consumed += serve_borek;
            borekconsumecount += serve_borek;
            if (serve_borek <= borek_tray) {
                borek_tray -= serve_borek;
            } else {
                borek_tray -= borek_tray;
            }
        }
        }    
    }
    if (randomei == 2) {
        if (slices_of_cake <= 0 && cake_tray == 0) {
            slices_of_cake = 0;
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        if (info->cake_consumed == MAX_CAKE) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        srand(time(NULL));
        serve_cake = rand() % 3;
        if(info->cake_consumed + serve_cake > MAX_CAKE){
            serve_cake = MAX_CAKE - info->cake_consumed;
        }
        if(serve_cake > cake_tray){
            serve_cake = cake_tray;
        }
        printf("Guest %d: Serving %d slices of cake\n", guest_id, serve_cake);
        info->cake_consumed += serve_cake;

        if (serve_cake <= cake_tray) {
            cake_tray -= serve_cake;
        } else {
            cake_tray -= cake_tray;
        }
    }
    if (randomei == 3) {
        if (glasses_of_drinks_number <= 0 && drink_tray == 0) {
            
            glasses_of_drinks_number = 0;
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        if (info->drink_consumed == MAX_DRINK) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        srand(time(NULL));
        serve_drink = rand() % 5;
        if(info->drink_consumed + serve_drink > MAX_DRINK){
            serve_drink = MAX_DRINK - info->drink_consumed;
        }
        if(serve_drink > drink_tray){
            serve_drink = drink_tray;
        }
        printf("Guest %d: Serving %d drinks\n", guest_id, serve_drink);
        info->drink_consumed += serve_drink;
        drinkconsumecount += serve_drink;

        if (serve_drink <= drink_tray) {
            drink_tray -= serve_drink;
        } else {
            drink_tray -= drink_tray;
        }
    }

    printf("guest %d consumed %d borek\n", guest_id, info->borek_consumed);
    printf("guest %d consumed %d cake\n", guest_id, info->cake_consumed);
    printf("guest %d consumed %d drink\n", guest_id, info->drink_consumed);

    printf("tray infos: borek=%d, cake=%d, drink=%d\n", borek_tray, cake_tray, drink_tray);
    printf("Remaining resources: Borek=%d, Cake=%d, Drink=%d\n", borek_number, slices_of_cake, glasses_of_drinks_number);
    pthread_mutex_unlock(&mutex);

    struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);

    randomei = foodPick();

    pthread_exit(NULL);
}

void update_food(int borek, int cake, int drink)
{
    borek_number = borek;
    slices_of_cake = cake;
    glasses_of_drinks_number = drink;

    printf("Remaining resources: Borek=%d, Cake=%d, Drink=%d\n", borek_number, slices_of_cake, glasses_of_drinks_number);
}

void *waiter_borek()
{
    pthread_mutex_lock(&mutex);
    if (borek_tray == 0) {
        printf("borek_tray fulled");
            if (borek_number <= borek_tray_max) {
            borek_tray = borek_number;
            update_food(0, slices_of_cake, glasses_of_drinks_number);
        }   else {
            borek_tray = borek_tray_max;
            update_food(borek_number - 5, slices_of_cake, glasses_of_drinks_number);
        }

     
    } else if (borek_tray == 1) {
        printf("borek_tray fulled");
        if (borek_number <= borek_tray_max) {
            if(borek_tray + borek_number> borek_tray_max){
                printf("borek_tray fulled");
                borek_tray = borek_tray_max;
                update_food(borek_number - 4, slices_of_cake, glasses_of_drinks_number);
            }
            else{
                borek_tray += borek_number;
           
            update_food(0, slices_of_cake, glasses_of_drinks_number);
            }

            }
    else{
        borek_tray = borek_tray_max;
        printf("borek_tray fulled");
        update_food(borek_number -4 , slices_of_cake , glasses_of_drinks_number);
        }
    }

    pthread_mutex_unlock(&mutex);
}

void *waiter_cake()
{
    pthread_mutex_lock(&mutex);
    if (cake_tray == 0) {
        printf("cake_tray fulled");
            if (slices_of_cake <= cake_tray_max) {
            cake_tray = slices_of_cake;
            update_food(borek_number, 0, glasses_of_drinks_number);
        }   else {
            cake_tray = cake_tray_max;
            update_food(borek_number, slices_of_cake - 5, glasses_of_drinks_number);
        }
}   else if (cake_tray == 1) {
        printf("cake_tray fulled");
        if (slices_of_cake <= cake_tray_max) {
            if(cake_tray + slices_of_cake> cake_tray_max){
                printf("cake_tray fulled");
                cake_tray = cake_tray_max;
                update_food(borek_number, slices_of_cake - 4, glasses_of_drinks_number);

                
            }
            else{
                        cake_tray += slices_of_cake;
            update_food(borek_number, 0, glasses_of_drinks_number); }
        }
        else{
        cake_tray = cake_tray_max;
        update_food(borek_number, slices_of_cake - 4, glasses_of_drinks_number);
        }
    }

    pthread_mutex_unlock(&mutex);
}

void *waiter_drink()
{
    pthread_mutex_lock(&mutex);
    if (drink_tray == 0) {
        printf("drink_tray fulled");
        if (glasses_of_drinks_number <= drink_tray_max) {

            drink_tray = glasses_of_drinks_number;
            update_food(borek_number, slices_of_cake, 0);
    }   else {
            drink_tray = drink_tray_max;
            update_food(borek_number, slices_of_cake, glasses_of_drinks_number - 5);
        }
}   else if (drink_tray == 1) {
        printf("drink_tray fulled");
        if (glasses_of_drinks_number <= drink_tray_max) {
            if(drink_tray + glasses_of_drinks_number> drink_tray_max){
                printf("drink_tray fulled");
                drink_tray = drink_tray_max;
                update_food(borek_number, slices_of_cake, glasses_of_drinks_number - 4);
            }
            else{
                drink_tray += glasses_of_drinks_number;
          
            update_food(borek_number, slices_of_cake, 0);
            }
    }   else {
            drink_tray = drink_tray_max;
            update_food(borek_number, slices_of_cake, glasses_of_drinks_number - 4);
        }
    }

    pthread_mutex_unlock(&mutex);
}

int main()
{
    while ((borek_number + slices_of_cake + glasses_of_drinks_number != 0) || (borek_tray + cake_tray + drink_tray != 0)) {
        if ((borek_number + slices_of_cake + glasses_of_drinks_number == 0) && (borek_tray + cake_tray + drink_tray == 0)) {
            for (int i = 0; i < num_guest; i++) {
        printf("Guest %d consumed %d borek\n", i, guest_info[i].borek_consumed);
        printf("Guest %d consumed %d cake\n", i, guest_info[i].cake_consumed);
        printf("Guest %d consumed %d drink\n", i, guest_info[i].drink_consumed);
        exit(0);
    }
        }
        srand(time(NULL));
        r = rand() % 8;
        

        printf("%d\n", r);

        if (borek_tray < 2 || cake_tray < 2 || drink_tray < 2) {
            waiter_borek();
        }

        if (cake_tray < 2) {
            waiter_cake();
        }

        if (drink_tray < 2) {
            waiter_drink();
        }

        GuestFunction guests[8] = {
            guestBehavior, // Guest 0
            guestBehavior, // Guest 1
            guestBehavior, // Guest 2
            guestBehavior, // Guest 3
            guestBehavior, // Guest 4
            guestBehavior, // Guest 5
            guestBehavior, // Guest 6
            guestBehavior  // Guest 7
        };

        guest_ids[r] = r;

       
        
        if (guest_info[r].borek_consumed == MAX_BOREK && guest_info[r].cake_consumed == MAX_CAKE && guest_info[r].drink_consumed == MAX_DRINK) {
            pthread_cancel(thread_ids[r]);
            printf("Guestd %d is cancelled", r);
            struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);

        }
        else
        {
            /*
            for (int i = 0; i < num_guest; i++) {
        printf("Guest %d consumed %d borek\n", i, guest_info[i].borek_consumed);
        printf("Guest %d consumed %d cake\n", i, guest_info[i].cake_consumed);
        printf("Guest %d consumed %d drink\n", i, guest_info[i].drink_consumed);
    }*/
    printf("tray infos: borek=%d, cake=%d, drink=%d\n", borek_tray, cake_tray, drink_tray);
             pthread_create(&thread_ids[r], NULL, guests[r], (void *)&guest_ids[r]);
        printf("thread");
        printf("%d", guests[r]);
        printf("created");
            pthread_join(thread_ids[r], NULL);
        struct timespec ts;
        ts.tv_sec = 1;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);

            
    
        }
    }

    printf("before thread\n");
    printf("%d\n", borek_number);


    for (int i = 0; i < num_guest; i++) {
        printf("Guest %d consumed %d borek\n", i, guest_info[i].borek_consumed);
        printf("Guest %d consumed %d cake\n", i, guest_info[i].cake_consumed);
        printf("Guest %d consumed %d drink\n", i, guest_info[i].drink_consumed);
    }
    pthread_join(thread_id, NULL);
    printf("after thread\n");
    return 0;
}
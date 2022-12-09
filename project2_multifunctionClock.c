#include<stdio.h>
#include<conio.h>
#include<stdlib.h>  //exit()
#include<windows.h> //sleep(seconds), Beep(frequency, durationInMilliSeconds)
#include<stdbool.h>
#include<time.h>    //time(), struct tm, time_t(datatype)

bool isValidTime(int h, int m, int s){

    if(h > 23 || m > 60 || s > 60) return false;
    else                           return true;
}
void playAlarm(){

    for(int i = 0; i< 10; i++) Beep(800, 500);
}
void showLiveTime(int h, int m, int s){

    while(true){

        system("cls");
        printf("\n\tTIME -> %02d : %02d : %02d\n", h, m, s);
        printf("\npress any key to go back...\n");
        if(kbhit()){ system("cls"); return; }

        s++;
        sleep(1);

        if(s > 59){ m++; s = 0; }
        if(m > 59){ h++; m = 0; }
        if(h > 23)  h = 1;
    }
}

void alarm(){

    printf("------------------ ALARM -------------------\n\n");

    //1. prompting the user to set alarm(s)...

    int alarmHour, alarmMin, aHour[24], aMin[61], alarmCount = 0;
    char choice;
    memset(aHour, 0, sizeof(int)*24);  /*for aHour[x], if x == 0 alarm is not set at that hour*/
    memset(aMin, 0, sizeof(int)*61);   /*for aMin[x], if x == 0 alarm is not set at that minute*/

    do{
        printf("\nEnter alarm %d (hour minute): ", alarmCount+1);
        scanf("%d %d", &alarmHour, &alarmMin);
        aHour[alarmHour] = 1;                   /*for aHour[x], if x == 1 alarm is set at that hour*/
        aMin[alarmMin]   = 1;                   /*for aMin[x], if x == 1 alarm is set at that minute*/
        alarmCount++;

        printf("set another alarm(y/n) : ");
        scanf(" %c", &choice);

    }while(choice == 'y');

    printf("\n  wait for the alarm(s) to be played...  or else"
           "\n  to return to main menu press ESC key..."
           "\n  press any other key if you wish to wait"
           "\n  note that returning to the main menu will cancel the alarm(s)!");

    if(getch() == 27){ system("cls"); return; }

    //2. playing the alarms accordingly...

    while(alarmCount){

        time_t s = time(NULL);
        struct tm *now = localtime(&s);

        int hour, min;
        hour = now->tm_hour;
        min  = now->tm_min;

        if(hour > 12) hour += 12;   /*converting time to 24 hour format if required*/

        if(aHour[hour] && aMin[min]){

            /* *******IMPORTANT NOTE*********
             the below 1 line is done to mark that current alarm is played, if not done-
             then alarm count might be decremeted multiple times for a single alarm which-
             will break the code
            */
            aMin[min]   = 0;

            playAlarm();
            alarmCount--;
        }
    }
}

void timer(){

    int h, m, s;

    printf("------------------ TIMER -------------------\n\n");
    printf("start timer for? (hour minute second) : " );
    scanf("%d%d%d", &h, &m, &s);

    if(isValidTime(h, m, s)){

        while(true){

            if(h == 0 && m == 0 && s == 0){
                playAlarm();
                return;
            }

            system("cls");
            printf("\n\t %02d : %02d : %02d\n", h, m, s);
            printf("\npress any key to cancel the timer and exit to main menu...\n");
            if(kbhit()){ system("cls"); return; }

            s--;
            sleep(1);

            if(s < 0){ m--; s = 59; }
            if(m < 0){ h--; m = 59; }
        }
    }
    else{
        puts("invalid value(s) entered!\n");
        puts("\npress any key to go back.. ");
        getch();
        system("cls");
        timer();        //doesn't affect execution since it is tail recursion
    }
}

void stopwatch(){

    printf("---------------- STOPWATCH -----------------\n\n");

    int h, m, s;
    h = m = s = 0;

    printf("\nPress 's' to start the stopwatch: ");
    if(getch() == 115){

        while(true){

            system("cls");
            printf("\n\t%02d : %02d : %02d\n", h, m, s);

            printf("\n  press any key to pause... ");
            if(kbhit()){

                int choice;
                printf("\n\n  press 1 to resume or 2 to return to main menu...");
                scanf("%d", &choice);
                switch(choice){
                case 1:
                    break;
                case 2:
                    system("cls");
                    return;
                }
            }

            s++;
            sleep(1);

            if(s > 59){ m++; s = 0; }
            if(m > 59){ h++; m = 0; }
            if(h > 23)  h = 1;

        }
    }
}

void goToClock(){

    int choice;

    while(true){

        printf("\n----------------- CLOCK ------------------\n"
               "\n\n1. display current time\n"
               "2. reset time and display the clock\n"
               "3. go back\n"
               "\nselect an option: ");

        scanf("%d", &choice);

        if(choice == 1){

            time_t s = time(NULL);                  //This will store the time in seconds
            struct tm *curr_time = localtime(&s);   //get the current time using localtime() function

            showLiveTime(curr_time->tm_hour, curr_time->tm_min, curr_time->tm_sec);
        }
        else if(choice == 2){

            //1. prompting user to reset the time...

            int h, m, s;
            printf("please reset the time(h m s) : ");
            scanf("%d%d%d", &h, &m, &s);

            //2. if the inputted values are valid then displaying the clock with the new time...

            if(isValidTime(h, m, s)) showLiveTime(h, m, s);
            else{
                puts("invalid value(s) entered!\n");
                puts("\npress any key to go back.. ");
                getch();
                system("cls");
                goToClock();     //doesn't affect execution since it is tail recursion
            }
        }
        else if(choice == 3){
            system("cls");
            return;
        }
        else{

            puts("\ninvalid choice!");
            puts("\npress any key to select again.. ");
            getch();
            system("cls");
            goToClock();        //doesn't affect execution since it is tail recursion
        }
    }
}

int main(){

    int choice;

    while(true){

        printf("--------------- MAIN MENU ----------------\n\n"
               "1. Clock\n"
               "2. Alarm\n"
               "3. Timer\n"
               "4. Stopwatch\n"
               "5. exit\n"
               "\nselect an option: ");

        scanf("%d", &choice);

        switch(choice){

        case 1:
            system("cls");
            goToClock();
            break;

        case 2:
            system("cls");
            alarm();
            break;

        case 3:
            system("cls");
            timer();
            break;

        case 4:
            system("cls");
            stopwatch();
            break;

        case 5:
            exit(1);

        default:
            puts("\ninvalid choice!");
            puts("\npress any key to select again.. ");
            getch();
            system("cls");
            main();        //doesn't affect exection since it is tail recursion
        }
    }

    return 0;
}

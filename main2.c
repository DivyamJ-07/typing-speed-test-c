#include<stdio.h>
#include<time.h>
#include<string.h>
#include <unistd.h> //unix header for using sleep()
#include <stdlib.h>
void test_start(char confirmation);
int count_correct_words(char *line, char *usentence);
float calc_wpm(int correct_words, float time);

char** load_lines(const char *filename, int *line_count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file\n");
        return NULL;
    }

    static char *lines[100];
    static char buffer[256];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        // Remove trailing newline if present
        char *newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';

        // Remove trailing comma if present
        char *comma = strchr(buffer, ',');
        if (comma) *comma = '\0';

        lines[count++] = strdup(buffer);  // copy line into static array
    }

    fclose(fp);
    *line_count = count;
    return lines;
}

int main(){
    srand(time(NULL));
    char confirmation;
    char uinput, usentence[100];
    char cwords[10][20], ucwords[10][20];
    
    printf("Welcome to a Typing Speed Test");
    sleep(1);
    printf("Do you wnat to test your Typing Speed?\nEnter N(no) or Y(yes): ");
    scanf("%c",&confirmation);
    if(confirmation=='Y'){
        test_start(confirmation);     //checks for confirmation from user before starting the test
    }
}

void test_start(char confirmation){
    printf("\nWelcome The Rules of the Typing Speed test are:\n1.You will be shown a line cotainign 10 words type it as fasts as possible.\n2.Words are Case sensitive.\n");
    sleep(5.0);
    int line_count = 0;
    char **lines = load_lines("fp.txt", &line_count);     //loading the lines from another .txt file

    if (lines == NULL) {
        printf("Error");
    }

    int rand_line=(rand()%line_count);
    char uinput;
    char usentence[100];     //user input sentence
    
    printf("Type this:\n%s\n", lines[rand_line]);
    printf("Press y when ready...");
    scanf(" %c", &uinput);
    getchar();
    float utime;
    if (uinput == 'y') {
        time_t start = time(NULL);
        fgets(usentence, 100, stdin);
        time_t end = time(NULL);
        utime = (float)end - start;
    }
    size_t len = strlen(usentence);
    if (len > 0 && usentence[len-1] == '\n')
    usentence[len-1] = '\0';      

    int correct_words = count_correct_words(lines[rand_line],usentence);
    
    float wpm = calc_wpm(correct_words,utime);
    printf("\nThe calculated WPM(Words per minute) of the user is %.2f", wpm);

}

int count_correct_words(char *line, char *usentence) {
    char temp[256];
    char words[10][20];
    int count = 0;

    //Copy the correct line into temp
    strcpy(temp, line);

    //Split correct line into words
    char *token = strtok(temp, " ");
    while (token != NULL && count < 10) {
        strcpy(words[count], token);
        count++;
        token = strtok(NULL, " ");
    }

    //Split user sentence
    char utemp[256];
    char ucwords[10][20];
    int ucount = 0;

    strcpy(utemp, usentence);

    char *utoken = strtok(utemp, " ");
    while (utoken != NULL && ucount < 10) {
        strcpy(ucwords[ucount], utoken);
        ucount++;
        utoken = strtok(NULL, " ");
    }

    //Compare
    int correct = 0;
    for (int i = 0; i < 10; i++) {
        if (strcmp(words[i], ucwords[i]) == 0) {
            correct++;
        }
    }

    return correct;
}

float calc_wpm(int correct_words, float time){
    float wpm = (float)correct_words/(time/60);
    return wpm;

}

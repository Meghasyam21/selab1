#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUBJECTS 5

struct Subject {
    char name[20];
    int minor; 
    int major;
    int total;
    char grade[3];
};

struct Student{
    char id[20];
    char name[30];
    struct Subject sub[SUBJECTS];
    int grand_total;
    float percentage;
    float cgpa;
};

// validation

int isAlphaNumeric(char id[]){
    for(int i=0; id[i];i++){
        if(!isalnum(id[i])) return 0;
    }
    return 1;
}

int isValidName(char s[]){
    for(int i=0; s[i];i++){
        if(!isalpha(s[i])) return 0;
    }
    return 1;
}

int isDuplicateID(struct Student s[], int n, char id[]){
    for(int i=0;i<n;i++)
        if(strcmp(s[i].id, id) == 0)
            return 1;
    return 0;
}

int validMinor(int m){
    return (m>=0 && m<=40);
}

int validMajor(int m){
    return (m>=0 && m<=60);
}

// computation

void grading(struct Subject *s){
    if(s->total >= 90) strcpy(s->grade, "O");
    else if(s->total >= 85) strcpy(s->grade, "A+");
    else if(s->total >= 75) strcpy(s->grade, "A");
    else if(s->total >= 65) strcpy(s->grade, "B+");
    else if(s->total >= 55) strcpy(s->grade, "C");
    else if(s->total >= 50) strcpy(s->grade, "D");
    else strcpy(s->grade, "F");
}

void Result(struct Student *s){
    s->grand_total = 0;

    for(int i=0;i<SUBJECTS;i++){
        s->sub[i].total = s->sub[i].minor + s->sub[i].major;
        grading(&s->sub[i]);
        s->grand_total += s->sub[i].total;

    }

    s->percentage = s->grand_total / 5.0;
    s->cgpa = s->percentage / 10.0; 

}

// input
int readFile(struct Student s[]){
    FILE *fp = fopen("input.txt","r");
    if(!fp){
        printf("input file not found\n");
        return 0;
    }

    char subjNames[SUBJECTS][20] = {"SE","Cloud","Algo","Virtu","DL"};
    int n;
    fscanf(fp,"%d",&n);

    for(int i=0;i<n;i++){
        fscanf(fp, "%s %s",s[i].id,s[i].name);
    

        if(!isAlphaNumeric(s[i].id) || isDuplicateID(s,i,s[i].id)){
            printf("Invalid/Duplicate id");
            i--;
            n--;
            continue;
        }

        for(int j=0;j<SUBJECTS;j++){
            strcpy(s[i].sub[j].name, subjNames[j]);

            fscanf(fp, "%d %d",&s[i].sub[j].minor,&s[i].sub[j].major);

            if(!validMinor(s[i].sub[j].minor) || !validMajor(s[i].sub[j].major)){
                printf("Invalid marks for %s\n",s[i].id);
                i--;
                n--;
                break;
            }
        }

        Result(&s[i]);
    }
    fclose(fp);
    return n;
}

void generateReport(struct Student s[],int n){
    FILE *fp = fopen("report.txt","w");

    fprintf(fp, "%-10s %-10s %-12s %-6s %-6s %-6s %-6s\n","ID","Name","Subject","Minor","Major","Total","Grade");

    for(int i=0;i<n;i++){
        for(int j=0;j<SUBJECTS;j++){
            fprintf(fp,"%-10s %-10s %-12s %-6d %-6d %-6d %6s\n",
            s[i].id,s[i].name,s[i].sub[j].name,s[i].sub[j].minor,s[i].sub[j].major,s[i].sub[j].total,s[i].sub[j].grade);
        }

        fprintf(fp,"Total: %d Percentage %.2f CGPA: %.2f\n",
        s[i].grand_total,s[i].percentage,s[i].cgpa);
    }

    fclose(fp);
}

int main(){

    struct Student s[100];
    int n = readFile(s);

    if(n>0){
        generateReport(s,n);
        printf("Report of students\n");
    }

    return 0;
}
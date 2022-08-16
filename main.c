#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <myconio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


#define F_QUESTION          "question.dat"
#define F_QCM               "qcm.dat"
#define F_QUESTIONNER       "questionner.dat"
#define F_PROPOSITION       "proposition.dat"


FILE* ptf_question      = NULL;
FILE* ptf_qcm           = NULL;
FILE* ptf_questionner   = NULL;
FILE* ptf_proposition   = NULL;
char global_username[100];


typedef struct _question question;
struct _question
{
    char    cde[14];
    char    texte[500];
};

typedef struct _qcm qcm;
struct _qcm
{
    int     num;
    char    username[100];
    struct  tm dte;
    int     uniquement_nouvelles;
    int     uniquement_ratees;

};

typedef struct _questionner questionner;
struct _questionner
{
    char    cde[14];
    int     num;
    char    numero[50];
    int     id;
};

typedef struct _proposition proposition;
struct _proposition
{
    int     id;
    char    texte[250];
    int     correcte;
    char    cde[14];
};


char get_validated_menu_choice(char*, int (*f)());
int validate_input(int user_input, int *, int );
void confirm_request_menu();
char* indentification_menu();
void main_menu();
void print_main_menu();
void data_modification_menu();
void print_data_modification_menu();
void proposition_menu();
void print_proposition_menu();
void qcm_menu();
void print_qcm_menu();
void question_menu();
void print_question_menu();
void questionner_menu();
void print_questionner_menu();
void print_examen_menu();
FILE* read_question();
void create_question();
void start_qcm_menu();
question* get_question_array();
proposition* get_proposition_array();
int get_count_from_stream(FILE*, size_t);
proposition* get_matching_propositions(char *, proposition *, int*);
int* get_ints();
questionner* get_matching_questionners(int , questionner *, int *);
questionner* get_questionner_array();
qcm* get_qcm_array();
void print_history_menu();
question* get_offered_questions(qcm , question *, proposition *, int , int *);
void open_files();
void close_files();
void shuffle(question *, int );
void create_proposition();
int get_proposition_id();
int get_qcm_num();
void read_all_proposition();
void read_all_question();
void print_question(question);
void read_all_qcm();
void print_qcm(qcm q);
void read_all_questionner();
void print_questionner(questionner *);
void print_proposition(proposition *);
void examen_menu();
void start_exam(int , qcm );
void write_qcm(qcm);
void write_questionner(questionner );
void score_screen(qcm );
int get_qcm_right_answer_count(int , int , proposition *, questionner *);
void history_menu();
void print_user_qcm_history();
int get_user_answered_quest_count();
void get_questionner_username(questionner , char *);
void print_single_qcm_history(qcm , int , int , proposition *, questionner *);
int get_qcm_question_count(int );
void question_history_menu();
void print_question_history(char *);
int get_proposition_answered_count(int );
void str_word_wrap (char *, int );
void identification_menu();
void remove_line_feed(char* );
void test(int test_id);
void remove_question();
void remove_proposition();


int main()
{

    open_files();
    strcpy(global_username, "DEBUG USER");
    srand(time(NULL)+30);
    main_menu();
    close_files();
}

//Ouverture des fichiers "question.dat", "proposition.dat", "questionner.dat", "qcm.dat". Si échec, alors création d'un nouveau fichier;
void open_files()
{
    ptf_question    = fopen(F_QUESTION,     "r+b") == NULL ? fopen(F_QUESTION,      "w+b") : fopen(F_QUESTION,      "r+b");
    ptf_qcm         = fopen(F_QCM,          "r+b") == NULL ? fopen(F_QCM,           "w+b") : fopen(F_QCM,           "r+b");
    ptf_questionner = fopen(F_QUESTIONNER,  "r+b") == NULL ? fopen(F_QUESTIONNER,   "w+b") : fopen(F_QUESTIONNER,   "r+b");
    ptf_proposition = fopen(F_PROPOSITION,  "r+b") == NULL ? fopen(F_PROPOSITION,   "w+b") : fopen(F_PROPOSITION,   "r+b");
}

//Fermeture des fichiers "question.dat", "proposition.dat", "questionner.dat", "qcm.dat".
void close_files()
{
    fclose(ptf_question     );
    fclose(ptf_qcm          );
    fclose(ptf_questionner  );
    fclose(ptf_proposition  );
}

//Mélange du tableau de question.
//Para 1 : tableau de question
//Para 2 : taille du tableau
//Ne retourne rien
void shuffle(question *questions, int size)
{
//Gère la création d'une nouvelle struct proposition. L'utilisateur saisie le contenue des variables. Ecrit la proposition dans le stream ptf_proposition
    if (size > 1)
    {
        int i;

        for (i = 0; i < size; i++)
        {
            int j = i + rand() / (RAND_MAX / (size-i) + 1);
            question q = questions[j];
            questions[j] = questions[i];
            questions[i] = q;
        }
    }
}
//Ne retourne rien
void create_proposition()
{
    fflush(stdin);
    proposition p;
    char buffer[250];
    do
    {
        clrscr();
        printf("**AJOUT PROPOSITION**\n\n");
        printf("Intitule de la proposition: (min 1 char - max 249 char)\n");
        fgets(buffer, 250, stdin);
    }
    while(strlen(buffer) < 1);
    //removes the mysterious \n appended to buffer.
    int len = strlen(buffer);
    if(buffer[len-1]=='\n')
        buffer[len-1]='\0';

    strcpy(p.texte, buffer);
    fflush(stdin);

    do
    {
        clrscr();
        printf("**AJOUT PROPOSITION**\n\n");
        printf("Code unique: (min 3 char - max 13 char)\n");
        fgets(buffer, 14, stdin);
    }
    while(strlen(buffer) < 3 || strlen(buffer) > 14);
    len = strlen(buffer);
    if(buffer[len-1]=='\n')
        buffer[len-1]='\0';
     strcpy(p.cde, buffer);
     fflush(stdin);
    do
    {
        clrscr();
        printf("**AJOUT PROPOSITION**\n\n");
        printf("Cette proposition est la bonne reponse:\n");
        printf("<1> Oui\n");
        printf("<0> Non\n");
        fgets(buffer, 2, stdin);
    }
    while(strchr("10", buffer[0])==NULL);
    //manually converting from ascii position to int
    p.correcte = buffer[0] - 48;
    fflush(stdin);

    p.id = get_proposition_id();
    fseek(ptf_proposition, 0, SEEK_END);
    fwrite(&p, sizeof(proposition), 1, ptf_proposition);
    proposition_menu();
}

//Lis le stream ptf_proposition, cherche l'id le plus élevé stocké et l'incrémente de 1.
//Si ptf_proposition ne contient pas de proposition, assigne 1 à l'id.
//Retourne l'id.
int get_proposition_id()
{
    int ret_val = 0;
    if (ptf_proposition == NULL)
    {
        ret_val = 1;
    }
    else
    {
        fseek(ptf_proposition, 0, SEEK_END);
        ret_val = ftell(ptf_proposition) / sizeof(proposition);
        ret_val++;
    }
    return ret_val;
}

//Gère la création d'une nouvelle struct question. L'utilisateur saisie le contenue des variables. Ecrit la question dans le stream ptf_question
//Ne retourne rien
void create_question()
{
    question q;
    char buffer[500];

    do
    {
        clrscr();
        printf("**AJOUT QUESTION**\n\n");
        printf("Texte de la question: (min 10 char - max 499 char)\n");
        fflush(stdin);
        fgets(buffer, 500, stdin);

    }
    while(strlen(buffer) < 10 || strlen(buffer) > 500);
    //removes the mysterious \n appended to buffer.
    int len = strlen(buffer);
    if(buffer[len-1]=='\n')
        buffer[len-1]='\0';
    strcpy(q.texte, buffer);

    do
    {
        clrscr();
        printf("**AJOUT QUESTION**\n\n");
        printf("Code unique: (min 3 char - max 13 char)\n");
        fflush(stdin);
        fgets(buffer, 14, stdin);
    }
    while(strlen(buffer) < 3 || strlen(buffer) > 14);
    len = strlen(buffer);
    if(buffer[len-1]=='\n')
        buffer[len-1]='\0';
    strcpy(q.cde, buffer);
    fflush(stdin);


    fseek(ptf_question, 0, SEEK_END);
    int write_result = fwrite(&q, sizeof(question), 1, ptf_question);

    if (write_result == 1)
    {
        question_menu();
    }
    else
    {
        printf("L'ecriture dans le stream ptf_question a échoué.\nFermeture du programme.");
    }
}

//Lis le stream ptf_qcm, cherche le num le plus élevé stocké et l'incrémente de 1.
//Si ptf_qcm ne contient pas de question, assigne 1 au num.
//Retourne le num.
int get_qcm_num()
{
    int highest_num     = 0,
    qcm_count           = get_count_from_stream(ptf_qcm, sizeof(qcm));
    qcm *qcms           = get_qcm_array();

    assert(ptf_qcm != NULL);

    for (int i = 0; i < qcm_count; i++)
    {
        if (qcms[i].num > highest_num)
            highest_num = qcms[i].num;
    }

    //Si qcms est vide, assigne 1 au premier qcm.
    if (highest_num == 0)
    {
        highest_num = 1;
    }
    else
    {
        highest_num++;
    }

    free_and_nullify(qcms);

    return highest_num;
}

//Affiche toutes les propositions du stream ptf_proposition
void read_all_proposition()
{
    proposition p;
    char buffer[100];
    clrscr();
    assert(ptf_proposition != NULL);
    fseek(ptf_proposition, 0, SEEK_SET);
    fread(&p, sizeof(proposition), 1, ptf_proposition);

    printf("**Lecture proposition.dat**\n\n");
    printf("-----\n");
    while (feof(ptf_proposition) == 0)
    {
        print_proposition(&p);
        fread(&p, sizeof(proposition), 1, ptf_proposition);
    }


    printf("Appuyez sur enter pour revenir en arriere.\n");
    fflush(stdin);
    getchar();
    proposition_menu();
}

//Affiche toutes les questions du stream ptf_question
void read_all_question()
{
    question q;
    char buffer[100];
    clrscr();
    assert(ptf_question != NULL);

    fseek(ptf_question, 0, SEEK_SET);
    fread(&q, sizeof(question), 1, ptf_question);

    printf("**Lecture question.dat**\n\n");
    printf("-----\n");
    while (feof(ptf_question) == 0)
    {
        print_question(q);
        fread(&q, sizeof(question), 1, ptf_question);
    }


    printf("Appuyez sur enter pour revenir en arriere.\n");
    fflush(stdin);
    getchar();
    question_menu();
}

//Affiche la question q
void print_question(question q)
{
    printf("Texte   :   \"%s\"\n", q.texte);
    printf("Cde     :   \"%s\"\n", q.cde);
    printf("-----\n");
}

//Affiche tous les qcm du stream ptf_qcm
void read_all_qcm()
{
    qcm q;
    char buffer[100];
    clrscr();
    assert(ptf_qcm != NULL);
    fseek(ptf_qcm, 0, SEEK_SET);
    fread(&q, sizeof(qcm), 1, ptf_qcm);

    printf("**Lecture qcm.dat**\n\n");
    printf("-----\n");
    while (feof(ptf_qcm) == 0)
    {
        print_qcm(q);
        fread(&q, sizeof(qcm), 1, ptf_qcm);
    }

    printf("Appuyez sur enter pour revenir en arriere.\n");
    fflush(stdin);
    getchar();
    qcm_menu();
}

//Affiche le qcm q
void print_qcm(qcm q)
{
    char buffer[100];
    printf("Num : %d\n", q.num);
    printf("Username : \"%s\"\n", q.username);
    strftime(buffer, 100, "%X - %d/%m/%Y", &q.dte);
    printf("Date : %s\n", buffer);
    printf("Uniquement nouvelles : %d\n", q.uniquement_nouvelles);
    printf("Uniquement ratees : %d\n", q.uniquement_ratees);
    printf("-----\n");
}

//Affiche tout les quesionner du stream ptf_questionner
void read_all_questionner()
{
    questionner q;
    char buffer[100];
    clrscr();
    assert(ptf_questionner != NULL);
    fseek(ptf_questionner, 0, SEEK_SET);
    fread(&q, sizeof(questionner), 1, ptf_questionner);

    printf("**Lecture questionner22.dat**\n\n");
    printf("-----\n");
    while (feof(ptf_questionner) == 0)
    {
        print_questionner(&q);
        fread(&q, sizeof(questionner), 1, ptf_questionner);
    }


    printf("Appuyez sur enter pour revenir en arriere.\n");
    fflush(stdin);
    getchar();
    questionner_menu();

}

//Affiche le questionner q
void print_questionner(questionner *q)
{
    printf("Cde : \"%s\"\n", q -> cde);
    printf("Num : %d\n", q -> num);
    printf("Numero : \"%s\"\n", q -> numero);
    printf("Id : %d\n", q -> id);
    printf("-----\n");
}

//Affiche la proposition p
void print_proposition(proposition *p)
{
    printf("Id : %d\n", p -> id);
    printf("Texte : \"%s\"\n", p -> texte);
    printf("Correcte : %d\n", p -> correcte);
    printf("Cde : \"%s\"\n", p -> cde);
    printf("-----\n");
}

//Lance le menu princpal
// 1 - pour s'identifier et ensuite ouvrir le menu qui permet de commencer un quizz et d'afficher les historiques
// 2 - pour accéder au opération CRD des fichiers bat
// 0 - pour fermer le programme
void main_menu()
{
    char choice = get_validated_menu_choice("012", print_main_menu);
    switch (choice)
    {
    case '1': identification_menu();
        break;
    case '2': data_modification_menu();
        break;
    }
}

//Affiche le menu principal
void print_main_menu()
{
    clrscr();
    printf("**MENU PRINCIPAL**\n\n");
    printf("<1> Commencer un quizz\n");
    printf("<2> Gerer les donnees\n");
    printf("<0> Quitter le programme\n\n-> ");
}

//Lance le menu qui permet de commencer un quizz et d'afficher les historiques
// 1 - pour commencer un quizz
// 2 - pour ouuvrir le menu d'historique
// 3 - pour revenir au menu principal
void examen_menu()
{
    char choice = get_validated_menu_choice("120", print_examen_menu);
    switch (choice)
    {
    case '1': start_qcm_menu();
        break;
    case '2': history_menu();
        break;
    case '0': main_menu();
    }
}
//Fonction qui gère:
// - l'affichage du qcm avec le menu de navigation
// - l'enregistrement de la struct qcm et des struct questionner associées
void start_exam(int requested_question_count, qcm var_qcm)
{

    int            currently_answered_questions    = 0,
                   offered_question_count          = 0,
                   cptr_current_question           = 0,
                   pos_x                           = 0,
                   pos_y                           = 0,
                   cptr_numero                     = 0,
                   question_count                  = get_count_from_stream(ptf_question, sizeof(question)),
                   cptr_right_answers              = 0,
                   available_question_count_after_filter = 0,
                   id_of_answers_size              = 0;
    size_t         match_count                     = 0;
    proposition    *propositions                   = get_proposition_array();
    question       *questions                      = get_question_array();
    question       *offered_questions;
    question       current_question;
    char           menu_choice;
    char           buffer[100];
    char           valid_menu_choices[100];
    int            id_of_answers[100];


    cptr_current_question   = 1;
    id_of_answers_size      = (int)(sizeof(id_of_answers)/sizeof(int));
    //Initializing array to 0, to be able to tell which questions are unanswered
    for (int i = 0; i < id_of_answers_size ; i++)
    {
        id_of_answers[i] = 0;
    }

    if (var_qcm.uniquement_nouvelles == 0 && var_qcm.uniquement_ratees == 0)
    {
        offered_questions = questions;
        available_question_count_after_filter = question_count;
    }
    else
    {
        offered_questions = get_offered_questions(var_qcm, questions, propositions, question_count, &available_question_count_after_filter);
    }

    /*for (int i = 0; i < available_question_count_after_filter; i++)
    {
        printf("\n%s", offered_questions[i]);
    }
    getchar();*/
    //makes sure enough questions are available in question.dat to satisfy user demands
    offered_question_count = requested_question_count > available_question_count_after_filter ? available_question_count_after_filter : requested_question_count;

    if (offered_question_count == 0)
    {
        clrscr();
        printf("Aucune question trouvee verifiant vos critere de selection des questions.\n");
        printf("Appuyez sur enter pour continuer\n");

        free_and_nullify(propositions);
        if (offered_questions != questions)
            free_and_nullify(offered_questions);
        free_and_nullify(questions);

        getchar();
        examen_menu();
    }
    else
    {
        //Tableau temporaire de questionners afin de stocker les questionner sans les écrire, au cas où l'utilisateur choisis abandonner et non sauvegarder.
        questionner *temp_questionners   = (questionner*) calloc(offered_question_count, sizeof(questionner));
        assert(temp_questionners != NULL);

        //randomizing the questions
        shuffle(offered_questions, available_question_count_after_filter);

        //Je dois printer un truc ici sinon tout est cassé, donc je print ça. Voila. Il y a des retour à la ligne, je sais pas pourquoi mais tout va bien.
        //Edit 17-juin : Apparement tout fonction maintenant
        //printf("coucou");

        do
        {
            clrscr();

            //Stocke la question à afficher
            current_question  = offered_questions[cptr_current_question - 1];

            proposition *matches = get_matching_propositions(current_question.cde, propositions, &match_count);
            strcpy(valid_menu_choices, "EA");

            printf("\rRepondu : %d sur %d\n\n", currently_answered_questions, offered_question_count);
            printf("Q%d %s\n\n", cptr_current_question, current_question.texte);


            for (int i = 0; i < match_count; i++)
            {
                printf("<%d> %s\n", i+1, matches[i].texte);
                valid_menu_choices[2+i] = (i + 1) +'0';
            }

            printf("\nReponse : %d", id_of_answers[cptr_current_question - 1]);
            pos_x = wherex();
            pos_y = wherey();
            printf("\r");
            gotoxy(30, pos_y);
            printf("<P>recedente      <S>uivante\n");
            gotoxy(30, pos_y+1);
            printf("<E>nregistrer     <A>bandonner\n\n");
            printf("-> ");
            //gotoxy(pos_x, pos_y);
            fflush(stdin);
            //printf("%s", valid_menu_choices);
            fgets(buffer, 3, stdin);

            //traitement questionner dans tableau temporaire temp_questionners
            // Si utilisateur appuie sur une réponse, enregistrer ou abandonner
            if (strchr(valid_menu_choices, toupper(buffer[0])) != NULL)
            {

                // Les bits sont zéroifié par calloc, num == 0 veut dire que le questionner n'a pas encore été gérée.
                //debug
//                int espion2 = var_qcm.num;
//                printf("\n %d", var_qcm.num);
//                int espion = temp_questionners[cptr_current_question - 1].num;
                //fin debug
                if (temp_questionners[cptr_current_question - 1].num == 0)
                {
                    strcpy(temp_questionners[cptr_current_question - 1].cde, current_question.cde);
                    temp_questionners[cptr_current_question - 1].num = var_qcm.num;
                    int index = (buffer[0] - '0') -1;
                    int getid = matches[index].id;
                    temp_questionners[cptr_current_question - 1].id = getid;
                    cptr_numero++;
                    char temp[10];
                    sprintf(temp, "%d", cptr_numero);
                    strcpy(temp_questionners[cptr_current_question - 1].numero, (temp));


                    currently_answered_questions++;

                    //tested and working
                    //print_questionner(&temp_questionners[cptr_current_question - 1]);
                }

                // Si l'utilisater appuie sur une des réponses
                if (toupper(buffer[0]) != 'P' && toupper(buffer[0]) != 's')
                {
                    id_of_answers[cptr_current_question - 1] = buffer[0] - '0';

                    if (cptr_current_question < offered_question_count)
                    {
                        cptr_current_question++;
                    }
                }
            }

            // Si utilisateur appuie sur S et que la question présente n'est pas la dernière
            if (toupper(buffer[0]) == 'S' && cptr_current_question < offered_question_count)
            {
                cptr_current_question++;
            }

            // Si utilisateur appuie sur P et que la question présente n'est pas la dernière
            if (toupper(buffer[0]) == 'P' && cptr_current_question > 1)
            {
                cptr_current_question--;
            }

            menu_choice = toupper(buffer[0]);

            free_and_nullify(matches);

        }while (strchr("EA", toupper(buffer[0])) == NULL);


        //Ecriture des données
        if (toupper(buffer[0]) == 'E')
        {
            fseek(ptf_qcm, 0, SEEK_END);
            int writing_result = fwrite(&var_qcm, sizeof(qcm), 1, ptf_qcm);
            assert(writing_result == 1);

            for (int i = 0; i < offered_question_count; i++)
            {
                //dimanche 16:38 -> bug compolation
                //writing_result =
                //assert(writing_result == 1);
                write_questionner(temp_questionners[i]);
            }
        }

        free_and_nullify(temp_questionners);
        free_and_nullify(propositions);
        if (offered_questions != questions)
            free_and_nullify(offered_questions);
        free_and_nullify(questions);


        score_screen(var_qcm);
    }



}

//Fonction qui trie les questions selon les critères de selectin choisis par l'utilisateur (uniquement ratées et uniquement nouvelles)
//Assigne à 'available_question_count_after_filter' la taille du tableau retourné.
//Retourne un tableau de struct question.
question* get_offered_questions(qcm q, question *questions, proposition *propositions, int question_count, int *available_question_count_after_filter)
{
    int             proposition_count           = get_count_from_stream(ptf_proposition, sizeof(proposition)),
                    cptr_question_match         = 0,
                    can_be_offered              = 0,
                    questionner_count           = get_count_from_stream(ptf_questionner, sizeof(questionner)),
                    qcm_count                   = get_count_from_stream(ptf_qcm, sizeof(qcm)),
                    questionner_match_count     = 0;
    questionner     *questionners               = get_questionner_array(),
                    *questionner_matches        = NULL;
    question        *question_match;
    qcm             *qcms                       = get_qcm_array();

    //debug
//    printf("\nDans exam\nNum : %d", q.num);
//    printf("\nNew : %d", q.uniquement_nouvelles);
//    printf("\nfailed: %d", q.uniquement_ratees);
//    getchar();
    //fin debug

    //Cette fonction ne devrait pas être appellée si cet assert n'est pas vérifié
    assert(!(q.uniquement_nouvelles ==0 && q.uniquement_ratees == 0));

    //boucle sur les questions
    for (int i = 0; i < question_count; i++)
    {
        can_be_offered = 1;
        //boucle sur les qcms
        for (int h = 0; h < qcm_count && can_be_offered == 1; h++)
        {
            if (strcmp(qcms[h].username, global_username) == NULL)
            {
                questionner_matches = get_matching_questionners(qcms[h].num, questionners, &questionner_match_count);
                assert(questionner_matches != NULL);
                //debug
                /*for (int g = 0; g < questionner_match_count; g++)
                {
                    print_questionner(&questionner_matches[g]);
                }
                getchar();*/
                //fin debug

                //boucle sur les questionner_match
                for (int j = 0; j < questionner_match_count && can_be_offered == 1; j++)
                {
                    if (q.uniquement_nouvelles == 1)
                    {

                        if (strcmp(questionner_matches[j].cde, questions[i].cde) == NULL)
                        {
                            can_be_offered = 0;
                        }
                    }

                    if (q.uniquement_ratees == 1 && can_be_offered == 1)
                    {
                        for (int k = 0; k < proposition_count && can_be_offered == 1; k++)
                        {
                            if (strcmp(questions[i].cde, propositions[k].cde) == NULL)
                            {
                                //test(questionner_matches[j].id);
                                /*printf("%questions[i].cde : %s", questions[i].cde);
                                printf("\nquestionner_matches[j].id : %d", questionner_matches[j].id);
                                printf("\npropositions[k].id : %d", propositions[k].id);
                                printf("\npropositions[k].correcte : %d", propositions[k].correcte);*/

                                if (questionner_matches[j].id == propositions[k].id && propositions[k].correcte == 1)
                                {
                                    can_be_offered = 0;
                                }
                                /*printf("\ncan_be_offered : %d", can_be_offered);
                                getchar();
                                printf("\n");*/
                            }
                        }
                    }
                }
                free_and_nullify(questionner_matches);
            }
        }
        if (can_be_offered == 1)
            cptr_question_match++;
    }
    if (cptr_question_match != 0)
    {
        question_match = (question*) calloc(cptr_question_match, sizeof(question));
        assert(question_match != NULL);

        cptr_question_match = 0;

        for (int i = 0; i < question_count; i++)
        {
            can_be_offered = 1;
            //boucle sur les qcms
            for (int h = 0; h < qcm_count; h++)
            {
                if (strcmp(qcms[h].username, global_username) == NULL)
                {
                    questionner_matches = get_matching_questionners(qcms[h].num, questionners, &questionner_match_count);

                    //boucle sur les questionner_match
                    for (int j = 0; j < questionner_match_count && can_be_offered == 1; j++)
                    {
                        if (q.uniquement_nouvelles == 1)
                        {

                            if (strcmp(questionner_matches[j].cde, questions[i].cde) == NULL)
                            {
                                can_be_offered = 0;
                            }
                        }

                        if (q.uniquement_ratees == 1 && can_be_offered == 1)
                        {
                            for (int k = 0; k < proposition_count && can_be_offered == 1; k++)
                            {
                                if (strcmp(questions[i].cde, propositions[k].cde) == NULL)
                                {
                                    if (questionner_matches[j].id == propositions[k].id && propositions[k].correcte == 1)
                                        can_be_offered = 0;
                                }
                            }
                        }
                    }
                    if (can_be_offered == 1)
                    {
                        question_match[cptr_question_match] = questions[i];
                        cptr_question_match++;
                    }

                    free_and_nullify(questionner_matches);
                }
            }
        }
    }
    else
    {
        question_match = NULL;
    }


    free_and_nullify(questionners);
    free_and_nullify(qcms);

    *available_question_count_after_filter = cptr_question_match;
    //affiche les cde des questions qui ont matché
    /*for (int i = 0; i < cptr_question_match; i++)
    {
        printf("\n%s", question_match[i]);
    }
    getchar();*/
    return question_match;
}

//Ecrit le qcm q dans le stream ptf_qcm
void write_qcm(qcm q)
{
    fseek(ptf_qcm, 0, SEEK_END);
    fwrite(&q, sizeof(qcm), 1, ptf_qcm);
}

//Ecrit le questionner q dans le stream ptf_questionner
void write_questionner(questionner q)
{
    fseek(ptf_questionner, 0, SEEK_END);
    fwrite(&q, sizeof(questionner), 1, ptf_questionner);
}

//Fonction appelée après start_exam().
//Affiche le score du qcm q (celui effectué à l'instant par l'utilisateur)
void score_screen(qcm q)
{
    int         match_count,
                proposition_count   = get_count_from_stream(ptf_proposition, sizeof(proposition)),
                right_answers       = 0,
                questionner_count   = get_count_from_stream(ptf_questionner, sizeof(questionner)),
                result              = -1;
    questionner *questionners       = get_questionner_array();
    questionner *matches            = get_matching_questionners(q.num, questionners, &match_count);
    proposition *propositions       = get_proposition_array();

    if (match_count == 0)
    {
        result = 0;
    }
    else
    {
        //Division par 0 impossible
        assert(match_count != 0);
        right_answers = get_qcm_right_answer_count(match_count, proposition_count, propositions, matches);
        result = ((right_answers*100)/match_count);
    }



    assert(proposition_count > 0);
    assert(questionner_count > 0);
    assert(result != -1);
    assert(right_answers != -1);

    clrscr();
    printf("Utilisateur     : %s\n", global_username);
    printf("QCM num         : %d\n", q.num);
    printf("Bonnes reponses : %d\n", right_answers);
    printf("Pourcentage     : %d%\n\n", result);

    free_and_nullify(matches);
    free_and_nullify(questionners);
    free_and_nullify(propositions);

    printf("Appuyez sur enter pour revenir en arriere.\n");
    fflush(stdin);
    getchar();

    examen_menu();
}

//Trouve le nombre de bonnes réponses d'un qcm.
//questionner_matches est un tableau qui contient les questionner associé au qcm dont on cherche le nombre de bonnes réponses.
//Retouen -1 si proposition_count est > 0;
int get_qcm_right_answer_count(int questionner_match_count, int proposition_count, proposition *propositions, questionner *questionner_matches)
{
    int cptr_right_answers = 0;

    //Should be impossible
    assert(questionner_match_count > 0);

    if (proposition_count > 0)
    {
        for (int i = 0; i < questionner_match_count; i++)
        {
            for (int j = 0; j < proposition_count; j++)
            {
                if (questionner_matches[i].id == propositions[j].id && propositions[j].correcte == 1)
                    cptr_right_answers++;
            }
        }
    }
    else
    {
        cptr_right_answers = -1;
    }
    return cptr_right_answers;

}


//Retourne un tableau de questionner en lisant le stream ptf_questionner
questionner* get_questionner_array()
{
    questionner     *questionners;
    questionner     q;
    int             questionner_count   = get_count_from_stream(ptf_questionner, sizeof(questionner)),
                    cptr                = 0;

    if  (questionner_count == 0)
    {
        return NULL;
    }

    questionners = (questionner*) calloc(questionner_count, (sizeof(questionner)));
    assert(questionners != NULL);



    fseek(ptf_questionner, 0, SEEK_SET);
    fread(&q, sizeof(questionner), 1, ptf_questionner);
    while(feof(ptf_questionner) == 0)
    {

        strcpy(questionners[cptr].cde, q.cde);
        questionners[cptr].id = q.id;
        questionners[cptr].num = q.num;
        strcpy(questionners[cptr].numero, q.numero);

        fread(&q , sizeof(questionner), 1, ptf_questionner);
        cptr++;
    }

    return questionners;
}

//Retourne un tableau de questionner associés au qcm dont le num = num_to_match
questionner* get_matching_questionners(int num_to_match, questionner *questionners, int *match_count)
{
    questionner *matches;
    int         count       = get_count_from_stream(ptf_questionner, sizeof(questionner)),
                cptr_match  = 0;
    questionner q;

    /*for (int i = 0; i < count; i++)
    {
        q = questionners[i];
        print_questionner(&q);
    }
    getchar();*/

    for (int i = 0; i < count; i++)
    {
        if (questionners[i].num == num_to_match)
        {
            cptr_match++;
        }
    }
    // so far so good - midnight sunday
    int size = cptr_match;
    matches = (questionner*) calloc (size, sizeof(questionner));
    assert(matches != NULL);

    cptr_match = 0;
    for (int i = 0; i < count; i++)
    {
        if (questionners[i].num == num_to_match)
        {
            matches[cptr_match] = questionners[i];
            cptr_match++;
        }
    }

    *match_count = cptr_match;

    return matches;
}


//Retourne une liste avec les propositions correspondant au CDE d'une question.
proposition* get_matching_propositions(char *pcde, proposition *propositions, int *match_count)
{
    proposition     *matches;
    int             count           = get_count_from_stream(ptf_proposition, sizeof(proposition)),
                    cptr_match      = 0;

    for (int i = 0; i < count; i++)
    {
        if (strcmp(propositions[i].cde, pcde) == NULL)
        {
            cptr_match++;
        }
    }
    int size = cptr_match;
    matches = (proposition*) calloc (size, sizeof(proposition));
    assert(matches != NULL);

    cptr_match = 0;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(propositions[i].cde, pcde) == NULL)
        {
            //PQ CA FAIT FOIRER
            strcpy(matches[cptr_match].cde, propositions[i].cde);
            matches[cptr_match].correcte = propositions[i].correcte,
            matches[cptr_match].id = propositions[i].id;
            strcpy(matches[cptr_match].texte, propositions[i].texte);
            cptr_match++;
        }
    }

    *match_count = cptr_match;

    return matches;
}

//Returns a question array containing all registered questions.
question* get_question_array()
{
    question    *questions;
    int         question_count  = get_count_from_stream(ptf_question, sizeof(question)),
                cptr            = 0;

    if (question_count == 0)
        return NULL;
    assert(question_count > 0);

    questions = (question*) calloc(question_count, (sizeof(question)));

    assert(questions != NULL);
    question q;

    fseek(ptf_question, 0, SEEK_SET);
    fread(&q, sizeof(question), 1, ptf_question);
    while(feof(ptf_question) == 0)
    {
        questions[cptr] = q;
        fread(&q , sizeof(question), 1, ptf_question);
        cptr++;
    }

    return questions;
}

//Retourne un tableau de proposition en llisant ptf_proposition
proposition* get_proposition_array()
{
    int          proposition_count  = get_count_from_stream(ptf_proposition, sizeof(proposition)),
                 cptr               = 0;

    if  (proposition_count == 0)
    {
        return NULL;
    }

    proposition  *propositions      = (proposition*) calloc(proposition_count, sizeof(proposition));
    proposition  p;

    assert(propositions != NULL);

    fseek(ptf_proposition, 0, SEEK_SET);
    fread(&p, sizeof(proposition), 1, ptf_proposition);
    while(feof(ptf_proposition) == 0)
        {
            propositions[cptr] = p;
            fread(&p , sizeof(proposition), 1, ptf_proposition);
            cptr++;
        }

    return propositions;
}

//Retourne le nombre d'élément de taille 'size' contenu dans le flux 'stream'
int get_count_from_stream(FILE *stream, size_t size)
{
    assert(stream != NULL);
    fseek(stream, 0, SEEK_END);
    int count = (ftell(stream) / size);
    return count;
}


//Demande à l'utilisateur : - d'entrer le nombre de questions qu'il souhaite
//                          - uniquement nouvelles ou pas
//                          - uniquement précédement ratées ou pas
void start_qcm_menu()
{
    long    requested_question_count;
    qcm     q;
    char    buffer[100];
    char    temp;
    do
    {
        clrscr();
        printf("**CONFIGURATION QCM - %s**\n\n", global_username);
        printf("Nombre de questions\r");
        gotoxy(50, 3);
        printf(":   ");
        fflush(stdin);
        fgets(buffer, 3, stdin);
    }while (strtol(buffer, NULL, 10) == 0);
    remove_line_feed(buffer);
    requested_question_count = strtol(buffer, NULL, 10);
    do
    {
        clrscr();
        printf("**CONFIGURATION QCM - %s**\n\n", global_username);
        printf("Nombre de questions\r");
        gotoxy(50, 3);
        printf(":   %d\n", requested_question_count);
        fflush(stdin);
        printf("Uniquement nouvelle (O/N)\r");
        gotoxy(50, 4);
        printf(":   ");
        fgets(buffer, 3, stdin);
    }while (strchr("ONon", buffer[0]) == NULL);
    q.uniquement_nouvelles = toupper(buffer[0]) == 'N' ? 0 : 1;

    if (q.uniquement_nouvelles == 0)
    {
        temp = buffer[0];
        do
        {
            clrscr();
            printf("**CONFIGURATION QCM - %s**\n\n", global_username);
            printf("Nombre de questions\r");
            gotoxy(50, 3);
            printf(":   %d\n", requested_question_count);
            printf("Uniquement nouvelle (O/N)\r");
            gotoxy(50, 4);
            printf(":   %c\n", temp);
            printf("Uniquement erronees dans le passe (O/N)\r");
            gotoxy(50, 5);
            printf(":   ");
            fgets(buffer, 3, stdin);
        }while (strchr("ONon", buffer[0]) == NULL);
        q.uniquement_ratees = toupper(buffer[0]) == 'N' ? 0 : 1;
    }
    else
    {
        q.uniquement_ratees = 0;
    }
    strcpy(q.username, global_username);
    // le temps bug
    time_t rawtime;
    time(&rawtime);
    q.dte = *localtime(&rawtime);
    q.num = get_qcm_num();

    //debug
//    printf("\nNew : %d", q.uniquement_nouvelles);
//    printf("\nFailed : %d", q.uniquement_ratees);
//    printf("\nNum : %d", q.num);
//    getchar();
    //end debug
    start_exam(requested_question_count, q);

}

//Affiche le menu examen_menu
void print_examen_menu()
{
    printf("**Examen - %s**\n\n", global_username);
    printf("<1> Repondre a un QCM\n");
    printf("<2> Acces historique\n");
    printf("<0> Retour\n");
}

//Lance le menu d'historique
void history_menu()
{
    int choice = get_validated_menu_choice("012", print_history_menu);
    switch (choice)
    {
    case '1': print_user_qcm_history();
        break;
    case '2': question_history_menu();
        break;
    case '0': examen_menu();
        break;
    }
}

//Affiche l'historique des qcm de l'utilisateur courant
void print_user_qcm_history()
{
    qcm         *qcms                       = get_qcm_array();
    int         qcm_count                   = get_count_from_stream(ptf_qcm,      sizeof(qcm)),
                total_question_count        = get_count_from_stream(ptf_question, sizeof(question)),
                user_answered_quest_count   = get_user_answered_quest_count(),
                match_count                 = 0,
                proposition_count           = get_count_from_stream(ptf_proposition, sizeof(proposition)),
                questionner_count           = get_count_from_stream(ptf_questionner, sizeof(questionner));
    questionner *questionners               = get_questionner_array();
    questionner *matches;
    proposition *propositions               = get_proposition_array();

    clrscr();
    printf("** Historique QCM - %s**\n\n", global_username);
    printf("%-16s%-10s%-12s%-12s%-10s\n", "Date", "Nb. q.", "Nouvelles", "Erreurs", "Score");

    //Affichage des QCM par ordre de date décroissant
    //for(int i = qcm_count - 1; i >= 0; i--)
    //Affichage des QCM par ordre de date croissant

    for(int i = 0; i < qcm_count; i++)
    {
        if (strcmp(qcms[i].username, strupr(global_username)) == NULL)
        {
            matches = get_matching_questionners(qcms[i].num, questionners, &match_count);
            //ici
            print_single_qcm_history(qcms[i], match_count, proposition_count, propositions, matches);
            free_and_nullify(matches);
        }
    }
    printf("\nQuestion deja posees     : %d", user_answered_quest_count);
    printf("\nQuestion jamais posees   : %d\n", (total_question_count - user_answered_quest_count));

    free_and_nullify(questionners);

    free_and_nullify(propositions);
    free_and_nullify(qcms);

    printf("\nAppuyez sur enter pour revenir en arriere.\n");
    fflush(stdin);
    getchar();

    history_menu();
}

//Retourne le nombre de question déjà posées à l'utilisateur
int get_user_answered_quest_count()
{
    question        *questions          = get_question_array();
    questionner     *questionners       = get_questionner_array();
    int             cptr                = 0,
                    has_been_answered   = 0,
                    question_count      = get_count_from_stream(ptf_question,    sizeof(question)),
                    questionner_count   = get_count_from_stream(ptf_questionner, sizeof(questionner));
    char            buffer[100];

    for (int i = 0; i < question_count; i++)
    {
        has_been_answered = 0;
        for (int j = 0; j < questionner_count && has_been_answered == 0; j++)
        {
            get_questionner_username(questionners[j], buffer);
            if (buffer != NULL)
            {
                if (strcmp((buffer), global_username) == NULL)
                {
                   if (strcmp(questions[i].cde, questionners[j].cde) == NULL)
                    {
                        cptr++;
                        has_been_answered = 1;
                    }
                }
            }
        }
    }

    free_and_nullify(questions);
    free_and_nullify(questionners);
    return cptr;
}

//Trouve le username correspondant à un questionner
//Nullify le pointeur du buffer si aucun username correspondant n'a été trouvé.
void get_questionner_username(questionner q, char *buffer)
{

    qcm   *qcms          = get_qcm_array();
    int   qcm_count      = get_count_from_stream(ptf_qcm,sizeof(qcm)),
          has_been_found = 0;

    for (int i = 0; i < qcm_count && has_been_found == 0; i++)
    {
        if (q.num == qcms[i].num)
        {
            strcpy(buffer, qcms[i].username);
            has_been_found = 1;
        }
    }

    if (has_been_found == 0)
    {
        buffer == NULL;
    }

    free_and_nullify(qcms);

}

//Affiche l'historique du qcm q
void print_single_qcm_history(qcm q, int match_count, int proposition_count, proposition *propositions, questionner *matches)
{
    char        buffer[100];
    char        score[100];
    int         right_answers   = get_qcm_right_answer_count(match_count, proposition_count, propositions, matches),
                i               = 0;
    double      result;

    //printf("%d / %d = ? ", right_answers, question_count);
    //getchar();

    //Division par 0 impossible
    assert(match_count != 0);

    result = (double)right_answers*100 / (double)match_count;
    sprintf(score, "%d (%.3g%%)", right_answers, result);
    strftime(buffer, 100, "%d/%m/%Y", &q.dte);

    if (right_answers == -1)
    {
        printf("Données du QCM corrompues.");
    }
    else
    {
        printf("%-16s%-10d%-12c%-12c%-10s\n", buffer, match_count, q.uniquement_nouvelles == 1 ? 'O' : 'N', q.uniquement_ratees == 1 ? 'O' : 'N', score);
    }
}

//Retourne le nombre de question posées lors d'un qcm dont le num == num_to_match
int get_qcm_question_count(int num_to_match)
{
    questionner     *questionners       = get_questionner_array();
    questionner     q;
    int             questionner_count   = get_count_from_stream(ptf_questionner, sizeof(questionner)),
                    cptr_match          = 0;

    for (int i = 0; i < questionner_count; i++)
    {
        q = questionners[i];
        if (questionners[i].num == num_to_match)
            cptr_match++;
    }

    free_and_nullify(questionners);
    return cptr_match;
}

//Retourne un tableau de qcm en lisant lle sttream ptf_qcm
qcm* get_qcm_array()
{
    qcm         *qcms;
    int         qcm_count  = get_count_from_stream(ptf_qcm, sizeof(qcm)),
                cptr       = 0;

    if  (qcm_count == 0)
    {
        return NULL;
    }

    assert(qcm_count > 0);

    qcms = (qcm*) calloc(qcm_count, (sizeof(qcm)));

    assert(qcms != NULL);
    qcm q;

    fseek(ptf_qcm, 0, SEEK_SET);
    fread(&q, sizeof(qcm), 1, ptf_qcm);
    while(feof(ptf_qcm) == 0)
    {
        qcms[cptr] = q;
        fread(&q , sizeof(qcm), 1, ptf_qcm);
        cptr++;
    }

    return qcms;
}

//Lance et le menu d'historique de question
void question_history_menu()
{
    char buffer[50];
    do
    {
        clrscr();
        printf("**Historique question - %s**\n\n", global_username);
        printf("Entrez un mot a rechercher dans les questions deja posees (min 1 char, max 39 char)\n");
        fgets(buffer, 40, stdin);
        remove_line_feed(buffer);
    }
    while(strlen(buffer) < 1 || strlen(buffer) > 40);
    char ret_val[50];
    strcpy(ret_val, buffer);
    print_question_history(ret_val);
    printf("Appuyez sur enter pour continuer");
    getchar();
    history_menu();
}

//Affiche l'historique de questions contenant le substrint 'substring'
void print_question_history(char *substring)
{
    question *questions      = get_question_array();
    int       question_count = get_count_from_stream(ptf_question, sizeof(question)),
              match_count,
              answered_count,
              pos_x          = 0,
              pos_y          = 0;
    char      buffer[501];
    char      s[100];
    strcpy(s,strupr(substring));

    clrscr();
    for (int i = 0; i < 10; i++)
    {
        printf("-----------");
    }
    printf("\n\n");

    for (int i = 0; i < question_count; i++)
    {
        strcpy(buffer, questions[i].texte);
        strupr(buffer);
        if (strstr(buffer, s) != NULL)
        {
            printf("Question %s\n\n", questions[i].cde);
            strcpy(buffer, questions[i].texte);
            str_word_wrap(questions[i].texte, 100);
            printf("\n");

            pos_y = wherey();
            gotoxy(60, pos_y);
            printf("Repondu");
            gotoxy(75, pos_y);
            printf("Correcte\n");

            pos_y = wherey();

            //gotoxy(60, pos_y);

            proposition *propositions   = get_proposition_array();
            proposition *matches        = get_matching_propositions(questions[i].cde, propositions, &match_count);

            for (int j = 0; j < match_count; j++)
            {
                pos_y = wherey();
                strcpy(buffer, matches[j].texte);
                printf("%d) ", j+1);
                str_word_wrap(buffer, 50);
                gotoxy(60, pos_y);
                answered_count = get_proposition_answered_count(matches[j].id);
                printf("%d", answered_count);
                if (matches[j].correcte == 1)
                {
                    gotoxy(75, pos_y);
                    printf("Oui");
                }
                printf("\n");
            }

            free_and_nullify(propositions);
            free_and_nullify(matches);
            printf("\n");
            for (int ii = 0; ii < 10; ii++)
            {
                printf("-----------");
            }
            printf("\n\n");
        }

    }
    free_and_nullify(questions);
}

//Retourne le nombre de fois que l'utilisateur courant a répondu proposition
int get_proposition_answered_count(int pid)
{
    questionner     *questionners           = get_questionner_array();
    int             questionner_count       = get_count_from_stream(ptf_questionner, sizeof(questionner));
    qcm             *qcms                   = get_qcm_array();
    int             qcm_count               = get_count_from_stream(ptf_qcm, sizeof(qcm)),
                    cptr_answered           = 0;

    for (int i = 0; i < qcm_count; i++)
    {
        char b1[200];
        char b2[200];
        strcpy(b1, qcms[i].username);
        strcpy(b2, global_username);
        if (strcmp(qcms[i].username, global_username) == NULL)
        {
            for (int j = 0; j < questionner_count; j++)
            {
                if (questionners[j].id == pid && qcms[i].num == questionners[j].num)
                {
                    cptr_answered++;
                }
            }
        }
    }
    free_and_nullify(questionners);
    free_and_nullify(qcms);

    return cptr_answered;

}

// fonction copiée de stackoverflow
// Pour wraper automatiquement le texte si les questions ou proposition sont trop longues
void str_word_wrap (char *buf, int n)
{
    char *p = buf,      /* pointer to current char */
        *sp = buf,     /* pointer to start of line to print */
        *ep = buf;     /* pointer to end of line to print */

    for (; *p && *p != '\n'; p++) { /* loop over each char (omit '\n')*/
        if (*p == ' ')              /* if space, set ep */
            ep = p;
        if (p - sp >= n) {          /* if wrap length 'n' reached */
            while (sp < ep)         /* loop outputting chars sp -> ep */
                putchar (*sp++);
            putchar ('\n');         /* tidy up with '\n' */
            sp = ++ep;              /* set start to next after end */
            ep = ++p;               /* set end to next after current */
        }
    }
    while (*sp && *sp != '\n')      /* output last line of chars */
        putchar (*sp++);
    putchar ('\n');                 /* tidy up with final '\n' */
}

//Affiche le menu historique
void print_history_menu()
{
    clrscr();
    printf("**Menu historique - %s**\n\n", global_username);
    printf("<1> Historique QCM\n");
    printf("<2> Historique questions\n");
    printf("<0> Retour\n");
}

//Lance le menu d'identification de l'utilisateur
void identification_menu()
{
    char buffer[100];
    do
    {
        clrscr();
        fflush(stdin);
        printf("**IDENTIFICATION UTILISATEUR**\n\n");
        printf("Veuillez entrer votre nom d'utilisateur  (min 3 char, max 99 char)\n");
        fgets(buffer, 100, stdin);
        fflush(stdin);
    }
    while(strlen(buffer) < 3 || strlen(buffer) > 100);
    int len = strlen(buffer);
    if(buffer[len-1]=='\n')
        buffer[len-1]='\0';
    strcpy(global_username, strupr(buffer));
    examen_menu();
}

//Lance le menu des opérattions CRD des fichiers dat
void data_modification_menu()
{
    int choice = get_validated_menu_choice("01234", print_data_modification_menu);
    switch (choice)
    {
    case '1': proposition_menu();
        break;
    case '2': qcm_menu();
        break;
    case '3': question_menu();
        break;
    case '4': questionner_menu();
        break;
    case '0': main_menu();
        break;
    }
}

//Affiche le menu des opération CRD des fichiers dat
void print_data_modification_menu()
{
    clrscr();
    printf("**GESTION DES DONNEES**\n\n");
    printf("<1> Proposition\n");
    printf("<2> Qcm\n");
    printf("<3> Question\n");
    printf("<4> Questionner\n");
    printf("<0> Retour\n\n-> ");
}

//Lance le menu d'opération CRD de proposition.dat
void proposition_menu()
{
    char valid_menu_choices[] = "0123";
    char choice = get_validated_menu_choice(valid_menu_choices, print_proposition_menu);
    switch (choice)
    {
    case '1': read_all_proposition();
        break;
    case '2': create_proposition();
        break;
    case '3': remove_proposition();
        break;
    case '0': data_modification_menu();
        break;
    }
}

//Affiche le menu d'opération CRD de proposition.dat
void print_proposition_menu()
{
    clrscr();
    printf("**PROPOSITION**\n\n");
    printf("<1> Afficher\n");
    printf("<2> Ajouter\n");
    printf("<3> Supprimer\n");
    printf("<0> Retour\n\n->");
}

//Lance le menu d'opération RD de qcm.dat
void qcm_menu()
{
    char choice = get_validated_menu_choice("012", print_qcm_menu);
    switch (choice)
    {
    case '1': read_all_qcm();
        break;
    case '2': data_modification_menu();
        break;
     case '0': data_modification_menu();
        break;
    }
}

//Affiche le menu d'opération RD de qcm.dat
void print_qcm_menu()
{
    clrscr();
    printf("**QCM**\n\n");
    printf("<1> Afficher\n");
    printf("<2> Supprimer\n");
    printf("<0> Retour\n\n->");
}

//Lance le menu d'opération CRD de question.dat
void question_menu()
{
    char choice = get_validated_menu_choice("0123", print_question_menu);
    switch (choice)
    {
    case '1': read_all_question();
        break;
    case '2': create_question();
        break;
    case '3': remove_question();
        break;
    case '0': data_modification_menu();
        break;
    }
}

//Affiche le menu d'opération CRD de question.dat
void print_question_menu()
{
    clrscr();
    printf("**QUESTION**\n\n");
    printf("<1> Afficher\n");
    printf("<2> Ajouter\n");
    printf("<3> Retirer\n");
    printf("<0> Retour\n\n->");

}

//Lance le menu d'opération RD de questionner.dat
void questionner_menu()
{
    char choice = get_validated_menu_choice("012", print_questionner_menu);
    switch (choice)
    {
    case '1': read_all_questionner();
        break;
    case '2': data_modification_menu();
        break;
    case '0': data_modification_menu();
        break;
    }
}

//Affiche le menu d'opération RD de questionner.dat
void print_questionner_menu()
{
    clrscr();
    printf("**QUESTIONNER**\n\n");
    printf("<1> Afficher\n");
    printf("<2> Supprimer\n");
    printf("<0> Retour\n\n->");
}

//Permet de récupérer un choix de menu valide.
//Recois le pointeur de la fonction menu à afficher
char get_validated_menu_choice(char *choices, int (*ptr_print_menu)())
{
    char buffer[100];
    do
    {
        clrscr();
        fflush(stdin);
        ptr_print_menu();
        fgets(buffer, 100, stdin);
        fflush(stdin);
    }
    while(strchr(choices, buffer[0]) == NULL);
    return toupper(buffer[0]);
}

//Retire le \n ajouté par la fonction fgets
void remove_line_feed(char* s)
{
    int len = strlen(s);
    if(s[len-1]=='\n')
        s[len-1]='\0';
}

//Libère le tableau passé en paramettre et assigne NULL
void free_and_nullify(void *array)
{
    if (array != NULL )
    {
       free(array);
       array = NULL;
    }

}

//Fonction de débuggage permettant d'afficher un int et de mettre le flux d'execution du programme en pause grâce à getchar
void test(int test_id)
{
    printf("\ntest %d", test_id);
    getchar();
}

//Retire une question uniquement si elle n'est pas référencée par un questionner pour ne pas avoir de fichier manquant dans l'affichage de l'historique
void remove_question()
{
    char            buffer[10];
    questionner     *questionners       = get_questionner_array();
    int             questionner_count   = get_count_from_stream(ptf_questionner, sizeof(questionner)),
                    question_count      = get_count_from_stream(ptf_question, sizeof(question)),
                    can_be_deleted      = 0,
                    does_question_exist = 0,
                    question_position   = 0,
                    validation          = 0,
                    cptr                = 0;
    question        *questions          = get_question_array(),
                    q,
                    q_temp;
    FILE            *new_stream;

    if (question_count != 0)
    {
        do
        {
            clrscr();
            printf("**SUPPESSION QUESTION**\n\n");
            printf("Entrez le CDE de la question a supprimer (uniquement si jamais repondue)\n");
            printf("<R>etour\n\n->");
            fflush(stdin);
            fgets(buffer, 10, stdin);
            remove_line_feed(buffer);
        }
        while(!(strlen(buffer) == 3 || toupper(buffer[0])== 'R'));

        for (int i = 0; i < question_count; i++)
        {
            if (strcmp(buffer, questions[i].cde) == NULL)
            {
                does_question_exist = 1;
                q = questions[i];
                question_position   = i;
            }
        }

        if (does_question_exist == 0)
        {
            clrscr();
            printf("Aucune question ne correspond au CDE : %s", buffer);
            printf("\nAppuyez sur enter pour revenir en arriere.\n");
            fflush(stdin);
            getchar();
        }
        else
        {
            assert(does_question_exist == 1);
            can_be_deleted = 1;
            for (int i = 0; i < questionner_count && can_be_deleted == 1; i++)
            {
                if (strcmp(questionners[i].cde, q.cde) == NULL)
                {
                    can_be_deleted = 0;
                }
            }
        }

        if (can_be_deleted == 1)
        {
            new_stream = fopen("temp.dat", "wb");
            for (int i = 0; i < question_count; i++)
            {
                fseek(ptf_question, i*sizeof(question), SEEK_SET);
                fseek(new_stream, 0, SEEK_END);
                if (i != question_position)
                {
                    validation = fread(&q_temp, sizeof(question), 1, ptf_question);
                    assert(validation == 1);
                    validation = fwrite(&q_temp, sizeof(question), 1, new_stream);
                    assert(validation == 1);
                }
            }
            //Vérifie la taille du nouveau stream;
            //fonctionne pas
            fseek(ptf_question, 0, SEEK_END);
            fseek(new_stream, 0, SEEK_END);

            //Vérifie que le nombre de bites est consistant
            assert(ftell(new_stream) == (ftell(ptf_question) - sizeof(question)));

            validation = fclose(new_stream);
            assert(validation == 0);
            new_stream = fopen("temp.dat",     "r+b") == NULL ? fopen("temp.dat",      "w+b") : fopen("temp.dat",      "r+b");




            // ***** rename et remove ne fonctionnent pas *****
            //char newname[] = "backup_question.dat";
            //assert(validation = rename(F_QUESTION, newname) == NULL);
            //rename(F_QUESTION, newname);
            //validation = remove(F_QUESTION);
            //erreur permission denied , peut-être car besoin droit admin pour supprimer à la racine du disque??
            //puts(strerror(errno));
            //test(1);
            //peut pas assigner null à int
            //assert(validation == NULL);

            //validation = rename("temp.dat", F_QUESTION);
            //peut pas assigner null à int
            //assert(validation == NULL);
            // ***** rename et remove ne fonctionnent pas *****

            validation = fclose(ptf_question);
            assert(validation == 0);

            ptf_question = fopen(F_QUESTION, "wb");

            //fseek(new_stream, 0, SEEK_END);
            //test(ftell(new_stream));

            fseek(new_stream, 0, SEEK_SET);

            question q_new;
            fread(&q_new, sizeof(question), 1, new_stream);
            /*print_question(q_new);
            test(22);*/
            while (feof(new_stream) == 0)
            {
                /*printf("\n%s\n", q_new.texte);
                getchar();*/
                //sécurité contre boucle d'écriture infinie
                assert(question_count > cptr);

                fseek(ptf_question, 0, SEEK_END);

                validation = fwrite(&q_new, sizeof(question), 1, ptf_question);
                assert(validation == 1);

                fread(&q_new, sizeof(question), 1, new_stream);
                cptr++;
            }

            validation = fclose(ptf_question);
            assert(validation == 0);
            ptf_question    = fopen(F_QUESTION,     "r+b") == NULL ? fopen(F_QUESTION,      "w+b") : fopen(F_QUESTION,      "r+b");

            validation = fclose(new_stream);
            assert(validation == 0);



            clrscr();
            printf("Question suivante supprimee : %s", buffer);
            printf("\nAppuyez sur enter pour revenir en arriere.\n");
            fflush(stdin);
            getchar();
        }
        else
        {
            clrscr();
            printf("Impossible de supprimer la question : %s", buffer);
            printf("\nAppuyez sur enter pour revenir en arriere.\n");
            fflush(stdin);
            getchar();
        }
    }
    else
    {
        clrscr();
        printf("Aucune question enregistree.");
        printf("\nAppuyez sur enter pour revenir en arriere.\n");
        fflush(stdin);
        getchar();
    }

    free_and_nullify(questions);
    free_and_nullify(questionners);

    question_menu();
}

//Retire une proposition uniquement si elle n'est pas référencée par un questionner pour ne pas avoir de fichier manquant dans l'affichage de l'historique
void remove_proposition()
{
    char            buffer[10];
    questionner     *questionners           = get_questionner_array();
    int             questionner_count       = get_count_from_stream(ptf_questionner, sizeof(questionner)),
                    proposition_count       = get_count_from_stream(ptf_proposition, sizeof(proposition)),
                    can_be_deleted          = 0,
                    does_proposition_exist  = 0,
                    proposition_position    = 0,
                    validation              = 0,
                    cptr                    = 0,
                    buffer_to_int           = 0;
    proposition     *propositions           = get_proposition_array(),
                    p,
                    p_temp;
    FILE            *new_stream;

    if (proposition_count != 0)
    {
        do
        {
            clrscr();
            printf("**SUPPESSION PROPOSITION**\n\n");
            printf("Entrez le ID de la proposition a supprimer (uniquement si jamais repondue)\n");
            printf("<R>etour\n\n->");
            fflush(stdin);
            fgets(buffer, 10, stdin);
            remove_line_feed(buffer);
        }
        while(!(strlen(buffer) > 0 || toupper(buffer[0])== 'R'));

        for (int i = 0; i < proposition_count; i++)
        {
            buffer_to_int = atoi(buffer);
            if (buffer_to_int == propositions[i].id )
            {
                does_proposition_exist = 1;
                p = propositions[i];
                proposition_position   = i;
            }
        }

        if (does_proposition_exist == 0)
        {
            clrscr();
            printf("Aucune proposition ne correspond au CDE : %s", buffer);
            printf("\nAppuyez sur enter pour revenir en arriere.\n");
            fflush(stdin);
            getchar();
        }
        else
        {
            assert(does_proposition_exist == 1);
            can_be_deleted = 1;
            for (int i = 0; i < questionner_count && can_be_deleted == 1; i++)
            {
                if (questionners[i].id == p.id)
                {
                    can_be_deleted = 0;
                }
            }
        }

        if (can_be_deleted == 1)
        {
            new_stream = fopen("temp.dat", "wb");
            for (int i = 0; i < proposition_count; i++)
            {
                fseek(ptf_proposition, i*sizeof(proposition), SEEK_SET);
                fseek(new_stream, 0, SEEK_END);
                if (i != proposition_position)
                {
                    validation = fread(&p_temp, sizeof(proposition), 1, ptf_proposition);
                    assert(validation == 1);
                    validation = fwrite(&p_temp, sizeof(proposition), 1, new_stream);
                    assert(validation == 1);
                }
            }
            //Vérifie la taille du nouveau stream;
            fseek(ptf_proposition, 0, SEEK_END);
            fseek(new_stream, 0, SEEK_END);

            //Vérifie que le nombre de bites est consistant
            assert(ftell(new_stream) == (ftell(ptf_proposition) - sizeof(proposition)));

            validation = fclose(new_stream);
            assert(validation == 0);
            new_stream = fopen("temp.dat",     "r+b") == NULL ? fopen("temp.dat",      "w+b") : fopen("temp.dat",      "r+b");

            validation = fclose(ptf_proposition);
            assert(validation == 0);

            ptf_proposition = fopen(F_PROPOSITION, "wb");

            fseek(new_stream, 0, SEEK_SET);
            proposition p_new;
            fread(&p_new, sizeof(proposition), 1, new_stream);

            while (feof(new_stream) == 0)
            {
                //sécurité contre boucle d'écriture infinie
                assert(proposition_count > cptr);

                fseek(ptf_proposition, 0, SEEK_END);

                validation = fwrite(&p_new, sizeof(proposition), 1, ptf_proposition);
                assert(validation == 1);

                fread(&p_new, sizeof(proposition), 1, new_stream);
                cptr++;
            }
            //so far so good
            validation = fclose(ptf_proposition);
            assert(validation == 0);
            ptf_proposition    = fopen(F_PROPOSITION,     "r+b") == NULL ? fopen(F_PROPOSITION,      "w+b") : fopen(F_PROPOSITION,      "r+b");

            validation = fclose(new_stream);
            assert(validation == 0);



            clrscr();
            printf("Proposition suivante supprimee : %d", buffer_to_int);
            printf("\nAppuyez sur enter pour revenir en arriere.\n");
            fflush(stdin);
            getchar();
        }
        else
        {
            clrscr();
            printf("Impossible de supprimer la proposition : %d", buffer_to_int);
            printf("\nAppuyez sur enter pour revenir en arriere.\n");
            fflush(stdin);
            getchar();
        }
    }
    else
    {
        clrscr();
        printf("Aucune proposition enregistree.");
        printf("\nAppuyez sur enter pour revenir en arriere.\n");
        fflush(stdin);
        getchar();
    }

    free_and_nullify(propositions);
    free_and_nullify(questionners);

    proposition_menu();
}

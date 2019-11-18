#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "pmake.h"


/* Print the list of actions */
void print_actions(Action *act) {
    while(act != NULL) {
        if(act->args == NULL) {
            fprintf(stderr, "ERROR: action with NULL args\n");
            act = act->next_act;
            continue;
        }
        printf("\t");

        int i = 0;
        while(act->args[i] != NULL) {
            printf("%s ", act->args[i]) ;
            i++;
        }
        printf("\n");
        act = act->next_act;
    }    
}

/* Print the list of rules to stdout in makefile format. If the output
   of print_rules is saved to a file, it should be possible to use it to 
   run make correctly.
 */
void print_rules(Rule *rules){
    Rule *cur = rules;
    
    while(cur != NULL) {
        if(cur->dependencies || cur->actions) {
            // Print target
            printf("%s : ", cur->target);
            
            // Print dependencies
            Dependency *dep = cur->dependencies;
            while(dep != NULL){
                if(dep->rule->target == NULL) {
                    fprintf(stderr, "ERROR: dependency with NULL rule\n");
                }
                printf("%s ", dep->rule->target);
                dep = dep->next_dep;
                printf("\n");
            }
            printf("\n");
            
            // Print actions
            print_actions(cur->actions);
        }
        cur = cur->next_rule;
    }
}


Action * create_action(char **arr, int n) {
    // create action nodes
    Action *act = malloc(sizeof(struct action_node));
    act->args = malloc((n + 1) * sizeof(char*));
    int c = 0;
    for(int i = 0; i < n; i++){
            (act->args)[i] = malloc(strlen(arr[i]) + 1);
            strcpy((act->args)[i], arr[i]);
            c++;
        
    }
    (act->args)[c] = NULL;
    return act;
}



Dependency * create_dependency(char **arr, Rule *head, int n) {
    
    Dependency *dep = malloc(sizeof(struct dep_node));
    Dependency *curr_dep = dep;
    
    // iterate through each dependency in array
    for(int i = 2; i  < n; i++) {
        
        Rule *curr = head;
        int found = 0;
        Rule *found_node;
        int c = 0;
        while(curr != NULL){
            c++;
            curr = curr->next_rule;
        }
        
        curr = head;
        while(curr != NULL){
            if(strcmp(curr->target, arr[i]) == 0){
                found = 1;
                found_node = curr;
                break;
            }
            curr = curr->next_rule;
        }
        if(found == 1){
            Dependency *new_dep = malloc(sizeof(struct dep_node));
            new_dep->rule = found_node;
            curr_dep->next_dep = new_dep;
            curr_dep = curr_dep->next_dep;
        } else {
            curr = head;
            // goes to last node
            while(curr != NULL && curr->next_rule != NULL){ 
                curr = curr->next_rule;
            }
            if(curr != NULL){

                    Rule *new_rule = malloc(sizeof(struct rule_node));
                    new_rule->next_rule = NULL;
                    new_rule->actions = malloc(sizeof(struct action_node));
                    new_rule->actions = NULL;
                    curr->next_rule = new_rule;
                    new_rule->target = malloc(strlen(arr[i]) + 1);
                    strcpy(new_rule->target, arr[i]);
                    Dependency *new_dep = malloc(sizeof(struct dep_node));
                    curr_dep->next_dep = new_dep;
                    new_dep->next_dep = NULL;
                    new_dep->rule = new_rule;
                    new_rule->dependencies = new_dep;
                    curr_dep = new_dep;
            }
        }
    // if dependency in head just add it to the dependency list to return
    // else add it to the END of head (last element) AND add it to the dependency list to return

    }
    return dep->next_dep;
}

void removeChar(char *str, char c) {
int i = 0;
int j = 0;

while (str[i]) {
    if (str[i] != c) {
        str[j++] = str[i];
      }
        i++;
    }
    str[j]='\0';
}
// Create dependency nodes given a list of tokenized words
    

Rule *parse_file(FILE *fp) {
    //use fgets to read from a file

    char line[MAXLINE];
    Rule *head = malloc(sizeof(struct rule_node));
    head->next_rule = NULL;
    head->target = "dummy";
    Action *curr_act = NULL;
    Rule *curr = head;
    int init = 0;
    //read one line at a time, and parse the line
    while(fgets(line, sizeof(line), fp) != NULL) {
        if(init != 0){
        }
        init = 1;

        // first use strtok() to split line into tokens
        // parse through tokens
        int n = 0;
        char *arr[MAXLINE];
        if(is_comment_or_empty(line) == 1){
            continue;
        }
        char *token = strtok(line, " ");
        if(token != NULL){
            removeChar(token, '\t');
            arr[n] = token;
            n++;
            token = strtok(NULL, " ");
        }
        while (token != NULL){
            removeChar(token, '\n');

            arr[n] = token;
            n++;
            token = strtok(NULL, " ");
 
            
        }
        int found = 0;
        
        for(int i = 0; i < n; i++) {

         if (strcmp(arr[i], ":") == 0){
                Rule *new_rule = malloc(sizeof(struct rule_node));
                new_rule->next_rule = NULL;
                new_rule->actions = malloc(sizeof(struct action_node));
                new_rule->actions = NULL;
                curr->next_rule = new_rule;
                new_rule->target = malloc(strlen(arr[0]) + 1);
                strcpy(new_rule->target, arr[0]);
                new_rule->dependencies = create_dependency(arr, head, n);
                curr = new_rule;
                found = 1;
                curr_act = NULL;
                break;

            }
        }
        if(found == 0){
            if(curr_act == NULL){
                curr->actions = create_action(arr, n);
                curr_act = curr->actions;
            }else{
                curr_act->next_act = create_action(arr, n);
                curr_act = curr_act->next_act;
            }

        }
    }
    return head->next_rule;
}


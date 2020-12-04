#include <stdio.h>
#include <stdlib.h>


int main() {
    int vertex=0;
    int node=0;
    int orientation=0;
    int weightiness=0;
    int c;
    char input[20];
    printf("Type name of input file:\n");
    scanf("%s", input);

    FILE *MatrixFile=fopen(input, "r");

    if (MatrixFile == NULL) {
        printf("No such File");
        exit(2);
    }
        while (!feof(MatrixFile)) {         // Считаем кол-во вершин
            if ((fgetc(MatrixFile)) == ';') {
                vertex++;
            }
        }
        fseek(MatrixFile, 0, SEEK_SET);

        while ((c = fgetc(MatrixFile)) != ';'){ // Кол-во узлов(связей)
            if (c == ',')
                node++;
        }
        node++;


        int Arr[vertex][node]; // Создание таблицы в виде двумерного массива
        fseek(MatrixFile, 0, SEEK_SET);
        for (int i=0; i < vertex; i++){
            int j=0;
            while ((fgetc(MatrixFile))!=';')
                fscanf(MatrixFile,"%d",&Arr[i][j++]);
        }
        fclose(MatrixFile);

        for (int i = 0; i < vertex; i++) {     // Проверка каждой колонки на взвешенность и ориентированность графа
            for (int j = 0; j < node; j++) {
                if (Arr[i][j]>1)
                    weightiness=1;
                if (Arr[i][j]<0)
                    orientation=1;
            }
        }

        int *weight[node]; // Если граф взвешен
        if (weightiness) {
            for (int j = 0; j < node; j++) {
                for (int i = 0; i < node; i++) {
                    if (Arr[i][j]>0)
                        weight[j]= &Arr[i][j];
                }
            }
        }

        char VertNames[vertex]; // Установка имен
        for (int i = 0; i < vertex; i++){
            VertNames[i]= 'a' + i;
        }
        char Connect[node][2]; // Установка связей между вершинами
        for (int j = 0; j < node; j++) {
            int k = 0;
            for (int i = 0; i < vertex; i++) {
                if (Arr[i][j]<0)
                    Connect[j][1] = VertNames[i];
                if (Arr[i][j]>0)
                    Connect[j][k++] = VertNames[i];
            }
        }

        FILE *GraphFile = fopen("graph.dot", "w"); // Создание dot-файла и преобразование его в png
        if (GraphFile == NULL) {
            printf("Problem with making a file");
            exit(2);
        }
        if (orientation)
            fprintf(GraphFile,"di");
        fprintf(GraphFile,"graph Test {\n");
        for (int i = 0; i < node; i++){
            if  (!orientation)
                fprintf(GraphFile,"%c -- %c ",Connect[i][0],Connect[i][1]);
            else {
                fprintf(GraphFile,"%c -> %c ",Connect[i][0],Connect[i][1]);
            }
            if (weightiness)
                fprintf(GraphFile,"[label = %d] \n",*weight[i]);
        }
        fprintf(GraphFile,"}\n");
        fclose(GraphFile);
        system("dot -Tpng graph.dot -o graph.png");

        printf("\nGraph is created\n");


        if (node>(((vertex-1)*(vertex-2))/2)) {  // Проверка связанности графа
            printf("\nThis graph is connected\n");
        }
        else {
            printf("\nThis graph is not connected\n");
        }

	printf("\nPlease check graph.dot and graph.png ");
    return 0;
}

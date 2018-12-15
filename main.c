#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <stdlib.h>
#include "LogisticaVerdeEscalonamento.h"
int main(int argc, char** argv){
    srand(atoi(argv[3]));
    // printf("%c\n",argv[2][0] );
    char metrica = argv[2][0];
    maquina* maquinas;
    job* jobs;
    char* nomeInstancia="instancias/m3n20/I9-m3n20-C.txt";//instancias/m8n1500/I1-m8n1500-A.txt";//"instancias/m100n200/I6-m100n200-B.txt";//"instancias/m8n1500/I1-m8n1500-A.txt";////"instancias/m10n1500/I4-m10n1500-A.txt";////"instancias/m8n1500/I1-m8n1500-A.txt";//"instancias/m8n1500/I3-m8n1500-A.txt";
    //printf("inicio");
    leInstancia(&maquinas,&jobs,argv[1]);//nomeInstancia);//argv[1]);//nomeInstancia);//
    //    for(int i=0;i < 20;i++){
//        printf("%d ", jobs[i].tempo);
//    }
    //makeSpanMin(maquinas,jobs,0);
//    construtivo1(maquinas,jobs,0.4);
//    printf("%f\n",(float)tempoTotalJobs(jobs)/(float)retNumMaq());
    maquina* sol = randReativo(maquinas,jobs,metrica);
    // construtivo1(maquinas, *jobs,0.5, tempoTotalJobs(jobs), int SpanMin, int custoMenor, int divisor, int flagCusto)

    /*printf("%d ",randomico(maquinas,jobs,0));*/
    //randomico(maquinas,jobs,0);
//    printf("acabeji ");
//    fflush(stdout);
//    heuristica(maquinas,jobs);
       /*preparaJobs(&jobs);   */
//    PrintLista(jobs);
    /*imprimeLista(jobs);*/
   PrintSaida(sol);
//    PrintSaida(maquinas);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "LogisticaVerdeEscalonamento.h"
#define BLOCK_ITERATIONS 25
#define ITERACOES 100
#define DELTA 10
int numJobs;
int numMaquinas;
int NUMJOBS;

int tempoTotalJobs(job* jobs){
    int i,cont=0;
    for( i=0;i < numJobs;i++){
        cont+=jobs[i].tempo;
    }
    return cont;
}

static int cmpCustoMaquina(const void *p1, const void *p2){
           return ((maquina*)p1)->custo > ((maquina*)p2)->custo;
       }
static int cmpTempoTotal(const void *p1, const void *p2){
           return ((maquina*)p1)->tempoTotal < ((maquina*)p2)->tempoTotal;
       }
static int cmpTempoJob(const void *p1, const void *p2){
           return ((job*)p1)->tempo < ((job*)p2)->tempo; //inverti<
       }
static int cmpInt(const void *p1, const void *p2){
           return *((int*)p1) < *((int*)p2);
       }

int inserirJob(job* jobCandidato,int m,maquina *maquinas){
//    printf("inserir %d ",jobCandidato->id );
    job* aux;//job* k=malloc(sizeof(job));
    if(maquinas!= NULL && jobCandidato!= NULL){
        aux = maquinas[m].job;
        maquinas[m].job = jobCandidato;
        jobCandidato->job = aux;
//        maquinas[m].job = malloc(sizeof(job));
//        maquinas[m].job->job = aux;
//        maquinas[m].job->tempo = jobCandidato->tempo;
//        maquinas[m].job->id = jobCandidato->id;
        maquinas[m].tempoTotal += jobCandidato->tempo;
        return 1;
    }else{
        printf("impossivel alocar");
        return 0;
    }
}

maquina* criaMaquinas(int *custoMaquina){
    int i;maquina* k=malloc(sizeof(maquina)*numMaquinas);
    for(i=0;i<numMaquinas;i++){
        k[i].id=i;
        k[i].custo =(double) custoMaquina[i];
        k[i].tempoTotal=0;
    }
    return k;
}

void leInstancia(maquina** maquinas, job** jobs, char* nomeInstancia ){
    FILE *instancia = fopen(nomeInstancia,"r");
    if(instancia !=NULL){
        fscanf(instancia,"%d\n",&numMaquinas);
        //printf("%d\n", numMaquinas);
        fscanf(instancia,"%d\n",&numJobs);
        //printf("%d\n", numJobs);
        //maquinas = malloc(sizeof(maquina*));
        //jobs = malloc(sizeof(job));
        *(maquinas)= malloc(sizeof(maquina)*numMaquinas);
        *(jobs)= malloc(sizeof(job)*numJobs);
        int i=0;
        do{
            //maquinas[i]=malloc(sizeof(maquinas));
            fscanf(instancia,"%d ",&(*maquinas)[i].custo);
            (*maquinas)[i].id=i;
            (*maquinas)[i].tempoTotal=0;
            i++;
        }while(i < numMaquinas);
        for(i=0;i<numJobs;i++) {
            fscanf(instancia,"%d\n",&(*jobs)[i].tempo);
            (*jobs)[i].id=i;
        }
        fscanf(instancia,"%d\n",&restricao);
      //  printf("%d\n",restricao);
    } else {
        printf("falhou na leitura");
    }
    //printf("%d\n",numJobs);
    for(int i =0 ; i < numMaquinas ; i++){
        (*maquinas)[i].job=NULL;
        (*maquinas)[i].tempoTotal=0;
    }
    NUMJOBS = numJobs;
}

double calculaMediaCusto(maquina *maquinas){
    int soma=0;
    for(int i=0;i< numMaquinas;i++){
        soma+=maquinas[i].custo;
    }
    printf("%f\n",(double)((double)soma/(double)numMaquinas) );
    return (double)((double)soma/(double)numMaquinas);
}

double calcBonus(maquina* m1,double custoMenor, double div,int itMaq){
        double saida;
        saida = exp((m1->custo - custoMenor)/div);
        return  2 - saida;
}

int retNumMaq(){
    return numMaquinas;
}

job* preparaJobs(const job* jobs){
   // qsort(jobs,numJobs,sizeof(job),cmpTempoJob);
    job* listaCandidatos = NULL;
    if(jobs != NULL){
        listaCandidatos= malloc(sizeof(job));
        listaCandidatos->id = jobs[0].id;
        listaCandidatos->job = NULL;
        listaCandidatos->tempo = jobs[0].tempo;
        job* cand = listaCandidatos;
        /*printf("%d %d\n",cand->tempo,cand->id);*/
        for(int i=1 ; i<numJobs ; i++){
            cand->job = malloc(sizeof(job));
            cand = cand->job;
            cand->id = jobs[i].id;
            cand->job = NULL;
            cand->tempo = jobs[i].tempo;
            //cand = cand->job;
        /*printf("%d %d\n",cand->tempo,cand->id);*/
        }
//    imprimeLista(listaCandidatos);
//    putchar('\n');
    }else{
        printf("impossivel vetor vazio");
    }
    return listaCandidatos;
}



job* extraiJob(job** jobs,int indice){
  job *aux = *jobs, *vo;
    if(jobs==NULL || *jobs==NULL){
       printf("nao consigo inserir Job\n");
       exit(29);
    }
   /*printf("%d\n", indice);fflush(stdout); */
    if(indice <= numJobs && indice >= 0){
        if(indice == 0){
          *jobs= (*jobs)->job;
          aux->job=NULL;
        }else{
            for(int i =0; i<= (indice-1) ; i++ ){
                vo = aux;
                aux = aux->job;
            }
            vo->job = aux->job;
        }
          numJobs--;
    }else
//        if(*jobs!=NULL){
//            return extraiJob( jobs, 0);
//        }else
            exit(3);
    return aux;
}

int retornaIndice(int lmc){
    int k = ( rand() % (numJobs) ) % (lmc+1);
    return k;
}

void construtivo1(maquina* maquinas,const job* jobs,double alfa, int tempoTotal,int SpanMin, int custoMenor,int divisor,int flagCusto){
    /*printf(" %f",alfa);*/
    /*fflush(stdout);        */
    numJobs = NUMJOBS;
    int itMaq=0,taxa=0;//custoTotal=0;//mediaCustos=calculaMediaCusto(maquinas);
    double aux;//int iteracao=0;
    job* listaCandidatos = preparaJobs(jobs);
    int lmc = ceil(alfa*numJobs);
    /*imprimeLista(listaCandidatos);*/
    job* cand = extraiJob(&listaCandidatos,retornaIndice(lmc));
    int ind;
    while(numJobs >= 0){//listaCandidatos != NULL){
        if((maquinas[itMaq].tempoTotal + cand->tempo) <= (SpanMin*( flagCusto ? calcBonus(&maquinas[itMaq],custoMenor, divisor,itMaq) : 1 ))+taxa){
            aux = maquinas[itMaq].custo * cand->tempo;
            if((calculaCustoTotal(maquinas) + aux) > restricao){      //otimizar
                if (listaCandidatos == NULL){
                    numJobs++;
                    listaCandidatos = cand;
                    taxa+=cand->tempo;
                    ind = retornaIndice(lmc);
                    cand = extraiJob(&listaCandidatos,ind);
                }else
                    taxa+=listaCandidatos->tempo;//mexer aqui talvez

                    viabilizaConstrutivo( maquinas,&listaCandidatos);
                itMaq=0;
            }else{
                if(inserirJob(cand,itMaq,maquinas) == 0){
                    exit(7);
                }
                if(listaCandidatos == NULL) break;
                //itJob++;
                ind = retornaIndice(lmc);
                cand = extraiJob(&listaCandidatos,ind);
                itMaq=0;
            }
        }else{
             if(itMaq >= numMaquinas-1){
                 /*if(listaCandidatos==NULL){*/
                    taxa+=cand->tempo;
                 /*}else*/
                    /*taxa+=listaCandidatos->tempo;*/
                    itMaq=0;
                }else{
                    itMaq++;
                }
        }
    }
    /*printf("sada\n");*/
    /*fflush(stdout);*/
    verificaTodosOsJobs(tempoTotal,maquinas);
    /*PrintSaida(maquinas);*/
}

// void selecionaConstrutivo(maquina** maquinas,job* jobs,double alfa, int tempoTotal,int SpanMin){
//     maquina* L = copiaMaq((*maquinas));
//     construtivo1((*maquinas),jobs,alfa,tempoTotal,SpanMin,(*maquinas)->custo,(*maquinas)[numMaquinas-1].custo,0);
//     construtivo1(L          ,jobs,alfa,tempoTotal,SpanMin,(*maquinas)->custo,(*maquinas)[numMaquinas-1].custo,1);
//     /*int l = calculaMakespan(*maquinas), k = calculaMakespan(L);*/
//     /*return ;*/
//     if(calculaMakespan(*maquinas) < calculaMakespan(L)){
//         destroiSolucao(L);
//     }else{
//         destroiSolucao(*maquinas);
//         *maquinas = L ;
//     }
// }

void selecionaMetrica(maquina** maquinas,job* jobs,double alfa, int tempoTotal,int spanMinTeo, int proporcao, char metrica){
    /*printf("dasd");*/
    /*fflush(stdout);*/
    switch (metrica) {
      case '0':{
          construtivo1((*maquinas),jobs,alfa,tempoTotal,spanMinTeo,(*maquinas)->custo,(*maquinas)[numMaquinas-1].custo,0);
          break;
      }
      case '1':{
          construtivo1((*maquinas),jobs,alfa,tempoTotal,spanMinTeo*proporcao,(*maquinas)->custo,(*maquinas)[numMaquinas-1].custo,0);
          break;
      }
      case '2':{
          construtivo1((*maquinas),jobs,alfa,tempoTotal,spanMinTeo,(*maquinas)->custo,(*maquinas)[numMaquinas-1].custo,1);
          break;
      }
      case '3':{
        construtivo1((*maquinas),jobs,alfa,tempoTotal,spanMinTeo*proporcao,(*maquinas)->custo,(*maquinas)[numMaquinas-1].custo,1);
        break;

      }
    }
    // int  metrica[]={spanMinTeo,spanMinTeo*proporcao,jobs[0].tempo};
    // maquina* solucoes[2];// = {copiaMaq(*maquinas),copiaMaq(maquinas)};
    // solucoes[0] = *maquinas;
    // solucoes[1] =  copiaMaq(*maquinas);
    // selecionaConstrutivo(&solucoes[0],jobs,alfa,tempoTotal,metrica[0]);
    // for(int i=1; i < 2; i++){
    //     /*printf("%d",i); */
    //     /*fflush(stdout);*/
    //     selecionaConstrutivo(&solucoes[1],jobs,alfa,tempoTotal,metrica[i]);
    //     if(calculaMakespan(solucoes[0]) > calculaMakespan(solucoes[1]) ) {
    //         destroiSolucao(solucoes[0]);
    //         solucoes[0] = solucoes[1];
    //     }else
    //         destroiSolucao(solucoes[1]);
    //
    // }
    // *maquinas = solucoes[0];
}

int calculaCustoTotal(maquina* maquinas){//devolve o custo total
    int count=0;
    for(int i=0;i<numMaquinas;i++){
        //printf(" %d--%d ",i,maquinas[i].tempoTotal );
        count+=maquinas[i].tempoTotal*maquinas[i].custo;
    }
//    printf("%d\n",count);
    return count;
}

int viabiliza(maquina* maquinas,job* jobs,int* itJob,int itMaq){//outro viabilizador, nao funciona
    int tempoReduzido=0;//i=0;
//    for(i=0; i<numJobs && maquinas[i].job!=NULL;i++){
//        ultimo=i;
//    }
    int ultimo=numMaquinas-1;

    while (maquinas[ultimo].job==NULL) {
        ultimo--;
    }
//    printf("%d \n",ultimo);
//    if(maquinas[itMaq].job == NULL){
//        return viabiliza(maquinas,jobs,itJob,itMaq-1);
//    }
    for(job* aux=maquinas[ultimo].job; aux != NULL ;aux=aux->job){
        //printf("\n");
        //imprimeLista(aux);
       // printf("\n%d-(%d)--%d---------------\n ",aux->id,aux->tempo,itMaq);
        //g                                                                                                                                       fflush(stdout);
        (*itJob)--;
        tempoReduzido+=aux->tempo;
        //printf("%d")
        jobs[(*itJob)].id=aux->id;
        jobs[(*itJob)].tempo=aux->tempo;
        jobs[(*itJob)].job=NULL;
        free(aux);
    }
    //tempoReduzido = maquinas[ultimo].tempoTotal;
    maquinas[ultimo].job = NULL;
    maquinas[ultimo].tempoTotal=0;
//    printf("tempo %d\n",tempoReduzido);
//    fflush(stdout);
    return tempoReduzido;
}

void viabilizaConstrutivo(maquina* maquinas,job** listaCandidatos){//executa metodo, onde, retira tudo da maquina mais custosa e devolve para a lista a ser alocada
//    int tempoReduzido=0,i=0;
//    for(i=0; i<numJobs && maquinas[i].job!=NULL;i++){
//        ultimo=i;
//    }
    if(listaCandidatos == NULL){
        printf("tentei viabilizar lista vazia\n");
        fflush(stdout);
        exit(29);
    }
    int ultimo = numMaquinas-1;
    while (maquinas[ultimo].job == NULL) {
        ultimo--;
    }
//    printf("%d \n",ultimo);

//    if(maquinas[itMaq].job == NULL){
//        return viabilizaConstrutivo(maquinas,listaCandidatos,itMaq-1);
//    }

//    for(job* aux=maquinas[ultimo].job; aux != NULL ;aux=aux->job){
    job* next = maquinas[ultimo].job;
    job* j;//*k;
    while(next!=NULL){
        //printf("\n");
        //imprimeLista(aux);
//        printf("\n%d-(%d)--%d---------------\n ",next->id,next->tempo,itMaq);
        //g
        //fflush(stdout);
//        (*itJob)--;
//        tempoReduzido+=aux->tempo;
//        printf("%d")
//        jobs[(*itJob)].id=aux->id;
//        jobs[(*itJob)].tempo=aux->tempo;
//        jobs[(*itJob)].job=NULL;
//        free(aux);
        j = *listaCandidatos;
        *listaCandidatos= extraiJob(&next,0);
        (*listaCandidatos)->job = j;
        numJobs++;
        numJobs++;
//        printf(" %d ",numJobs);
//        fflush(stdout);
//        *listaCandidatos = next;
//        next->job = j;
        //swap->job=NULL;
        //aux->job = NULL;
//        next=k;
    }
    //tempoReduzido = maquinas[ultimo].tempoTotal;
    maquinas[ultimo].job = NULL;
    maquinas[ultimo].tempoTotal=0;
//    printf("tempo \n");
//    fflush(stdout);
}

int otimizacao(maquina* maquinas,int ind,int i){//achar a melhor troca,,,, acrescentar
    int custoAntes= maquinas[ind].custo*maquinas[ind].tempoTotal + maquinas[i].custo*maquinas[i].tempoTotal;
    //printf("%d\n",custoAntes);
    //printf("%d\n----\n",custoSimulado);
    int melhorTroca=i;
    int custoSimulado= custoAntes;
    while(i >= 0){
        int custoCandidato= maquinas[ind].custo*maquinas[i].tempoTotal + maquinas[ind].custo*maquinas[i].tempoTotal;
        if(custoSimulado > custoCandidato){
            melhorTroca = i;
            custoSimulado = custoCandidato;
            printf("%d\n----\n",custoSimulado);
        }
        i--;
    }
    job* aux = maquinas[ind].job;
    maquinas[ind].job = maquinas[i].job;
    maquinas[i].job = aux;
    printf("%d\n",custoAntes);
                printf("%d\n----\n",custoSimulado);

    int res = maquinas[ind].tempoTotal;
    maquinas[ind].tempoTotal = maquinas[melhorTroca].tempoTotal;
    maquinas[melhorTroca].tempoTotal = res;
    return  custoAntes - (maquinas[ind].custo*maquinas[melhorTroca].tempoTotal + maquinas[ind].custo*maquinas[melhorTroca].tempoTotal);
}

void otimizacao2(maquina* maquinas){//ordena pra tentar melhorar custo
    for(int i=0;i<numMaquinas;i++){
        for(int j=numMaquinas-1;i!=j;j--){
            if(maquinas[i].tempoTotal < maquinas[j].tempoTotal){
                job* aux = maquinas[i].job;
                int aux1 = maquinas[i].tempoTotal;
                maquinas[i].job= maquinas[j].job;
                maquinas[i].tempoTotal= maquinas[j].tempoTotal;
                maquinas[j].job= aux;
                maquinas[j].tempoTotal= aux1;
            }
        }
    }

}


void verificaTodosOsJobs(int tempoTotal,maquina* saida){
    int i;job *next = NULL;
    int aux=0;
    int count=0;
    for(i=0; i < numMaquinas ;i++){
//         printf("maquina %d, custo %d ",said  a[i].id,saida[i].custo);

        count+=saida[i].tempoTotal;
        if(saida[i].job!=NULL){
            next=saida[i].job;
        }
        while(next != NULL) {
//                 printf("-> %d-(%d) ",next->id,next->tempo);
            aux+=next->tempo;
            next=next->job;
        }
//        if(makespawn<aux){
//            makespawn=aux;
//        }
        //aux*=saida[i].custo;
//         printf("\n");
    }
   if(aux == tempoTotal && count == tempoTotal){
        //printf("\nfunciona considerando tudo  %d---%d\n",aux,tempoTotal);

    }else{
        PrintSaida(saida);
        printf("\nestou com problemas considerando tudo, %d---%d\n",aux,tempoTotal);
        exit(13);
    }
}



int calculaCusto(maquina* saida){//calcula custo máquina a maq
  int i;job *next;int custo=0,aux=0;
  if(saida==NULL)
      exit(12);
  for(i=0;i<numMaquinas;i++){
      aux=0;
      if(saida[i].job!=NULL){
          next=saida[i].job;
      }
      while (next!=NULL) {
          aux+=next->tempo;
          next=next->job;
      }
      aux*=saida[i].custo;
      custo+=aux;
  }
  return custo;
}

void PrintSaida(maquina* saida){// gera saida formatada
    int i;job *next = NULL;int custo=0,aux=0,makespawn=0;
     /*printf("saida:\n");*/
    for(i=0; i < numMaquinas ;i++){
        aux=0;
         /*printf("\nmaquina %d, custo %d, tempoTotal %d : \n",saida[i].id,saida[i].custo,saida[i].tempoTotal);*/
        if(saida[i].job!=NULL){
            next=saida[i].job;
        }
        while(next != NULL) {
            /*printf("-> %d-(%d) ",next->id,next->tempo);*/
            aux+=next->tempo;
            next=next->job;
        }
        if(makespawn<aux){
            makespawn=aux;
        }
        if(aux != saida[i].tempoTotal){
//            printf("deu problema feio");
            exit(12);
        }

        aux*=saida[i].custo;
        custo+=aux;
        /*printf("\n");*/
    }
    printf("c: %d\n",custo);
    printf("R: %d\n",restricao);
    printf("m: %d\n",makespawn);
    printf("j: %d\n",NUMJOBS);
    printf("M: %d\n",numMaquinas);
    fflush(stdout);
}

maquina* copiaMaq(maquina* maquinas){//faz copia da solução
    maquina* maq = malloc(sizeof(maquina)*numMaquinas);
    for(int i =0 ; i< numMaquinas;i++){
        maq[i].custo = maquinas[i].custo;
        maq[i].id = maquinas[i].id;
        maq[i].job = maquinas[i].job;
        maq[i].tempoTotal = maquinas[i].tempoTotal;
    }
    return maq;
}

void destroiLista(job* jobs){// exclui lista alocada na maquina
    job * aux;
    for(job* next= jobs; next != NULL ; ){
        aux = next;
        next = next->job;
        free(aux);
        aux = NULL;
    }
}

void destroiSolucao(maquina* maquinas){//destroi toda uma soluçao
    for(int i =0 ; i< numMaquinas;i++){
//         printf("deu ruim");
//                    fflush(stdout);
        destroiLista(maquinas[i].job);
    }
    free(maquinas);
    maquinas = NULL;
}

int calculaMakespan(maquina* maquinas){//retorna o maior tempo total, nas máquinas, se alocada corretamente
    int maior = maquinas[0].tempoTotal;
    for(int i = 1; i < numMaquinas ; i++){
        if( maior < maquinas[i].tempoTotal )
            maior = maquinas[i].tempoTotal;
    }
    return maior;
}

int sorteiaIndice(double *prob){
  int sorteado = 0 ;
  double a = rand() % 100001;
  a /= 100000;
  // printf("%lf\n", a);
  // double a = rand()%melhoras;
  // printf("rand %lf\n",a );
  // printf("vetor de probabilidades:\n" );
  // for (int i = 0; i < 10; i++) {
      // printf("(%lf) ",prob[i]);
  // }
  for(double o = a; sorteado < 9 && o > 0  ;sorteado++, o-= prob[sorteado]);
  //   if (sorteado == 0) {
  //   printf("%d-",sorteado);
  // }
     //percorre para achar lugar sorteado
  // printf("\n");
  // printf("sorteado: %d\n",sorteado );
  // fflush(stdout);
  return sorteado;
}

maquina* randReativo(maquina* maquinas, job* jobs,char metrica){//metodo randReativo
    qsort(maquinas,numMaquinas,sizeof(maquina),cmpCustoMaquina);
    qsort(jobs,numJobs,sizeof(job),cmpTempoJob);
    int proporcao = jobs[NUMJOBS-1].tempo/jobs->tempo;
    int tempoTotal= tempoTotalJobs(jobs);
    int SpanMin=ceil((tempoTotal/numMaquinas));//*(maquinas[numMaquinas-1].custo/maquinas->custo));//*(1+taxa);
    maquina* melhorSolucao = copiaMaq(maquinas);
    // maquina* solucao = copiaMaq(maquinas);
    double alfa[] = {0.05,0.1,0.15,0.2,0.25,0.30,0.35,0.40,0.45,0.5};
    int qtd[10] = {0,0,0,0,0,0,0,0,0,0};
    double prob[10] = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};
    unsigned MakeSpantTotalPorAlfa[] = {0,0,0,0,0,0,0,0,0,0};
    double q[10] = {0,0,0,0,0,0,0,0,0,0};
    // int melhoras = 10;
    int sorteado= sorteiaIndice(prob);
    selecionaMetrica( &melhorSolucao, jobs,  sorteado, tempoTotal, SpanMin, proporcao,metrica);
    /*construtivo1()*/
    int melhormakeSpan = calculaMakespan(melhorSolucao);
    qtd[sorteado]++;
    MakeSpantTotalPorAlfa[sorteado] += melhormakeSpan;
    /*printf("asdas\n");*/
    /*construtivo2( melhorSolucao, jobs,  alfa[rand()%melhoras], tempoTotal, SpanMin); */
    /*PrintSaida(melhorSolucao);*/
    /*fflush(stdout);*/
    /*exit(0);*/
    int vezesSemMelhora = 1;
    int iteracoes = 1;
    while(vezesSemMelhora < ITERACOES){
        sorteado= sorteiaIndice(prob);
        maquina* solucao = copiaMaq(maquinas);
        // printf("alfa : %lf\n",alfa[sorteado] );
        selecionaMetrica( &solucao, jobs,  alfa[sorteado], tempoTotal, SpanMin, proporcao, metrica);
        // printf("%d\n",vezesSemMelhora );
        // fflush(stdout);
        // construtivo2 ( &solucao, jobs,  alfa[sorteado], tempoTotal, SpanMin, proporcao);
        // if(calculaMakespan(melhorSolucao) > calculaMakespan(solucao)){
        int makeSpanSolucao = calculaMakespan(solucao);
        MakeSpantTotalPorAlfa[sorteado] += makeSpanSolucao;
        qtd[sorteado]++;
        if( melhormakeSpan > makeSpanSolucao ){
            // prob[sorteado]++;
            // melhoras++;
            melhormakeSpan = makeSpanSolucao;
            destroiSolucao(melhorSolucao);
            melhorSolucao = solucao;
            vezesSemMelhora = 0;
            /*PrintSaida(solucao);*/
        }else{
            destroiSolucao(solucao);
            vezesSemMelhora++;
        }
        if (iteracoes%BLOCK_ITERATIONS == 0){
          double fatorNorma = 0;
          for (int i = 0; i < 10; i++) {
            if(qtd[i] != 0){
            q[i]=pow(melhormakeSpan/(((float)MakeSpantTotalPorAlfa[i])/qtd[i]),DELTA);
            fatorNorma+= q[i];
            // printf("q: %lf", q[i]);
            // fflush(stdout);
            }
          }                    
          for (int i = 0; i < 10; i++) {
            prob[i] = q[i]/fatorNorma;
          }
        }
        iteracoes++;
    }
    return melhorSolucao;
}

void imprimeLista(job* jobs){
    if(jobs==NULL){
       printf("lista Vazia");
       fflush(stdout);
    }
    else{
       for(job* L = jobs; L != NULL; L=L->job){
          printf("%d-(%d) ",L->id,L->tempo);
       }
       fflush(stdout);
    }
}

int inserirJobSimples(job* jobs, int m, maquina* maquinas){
//    printf("\n%d\n",m);
//    imprimeLista(maquinas[m].job);
    if(m< numMaquinas && jobs!= NULL){//entrando kras repetidos
        job* aux = maquinas[m].job;
        maquinas[m].job = malloc(sizeof(job));
        maquinas[m].job->id = jobs->id;
        maquinas[m].job->tempo = jobs->tempo;
        maquinas[m].job->job = aux;
//        printf("custoJob: %d \n",jobs->tempo*maquinas[m].custo );
        maquinas[m].tempoTotal+=jobs->tempo;
//        printf("%d \n",maquinas[m].tempoTotal );
//        imprimeLista(maquinas[m].job);    putchar('\n');

    }else{
        exit(9);
    }
    return 0;
}

void reservaCandidato(job** listaCandidatos,job** candidato,int* numCandidatos){
    (*candidato)->job = *listaCandidatos;
    *listaCandidatos = *candidato;
    /*numJobs--;*/
    (*numCandidatos)++;
}

/*void construtivo2(maquina* maquinas,const job* jobs,double alfa, int tempoTotal,int SpanMin,int taxa,int flag){*/
    /*[>printf(" %f",alfa);<]*/
    /*[>fflush(stdout); <]*/
    /*numJobs = NUMJOBS;*/
    /*int itMaq=0,taxa=0;//custoTotal=0;//mediaCustos=calculaMediaCusto(maquinas);*/
    /*double aux;//int iteracao=0;*/
    /*job* listaCandidatos = preparaJobs(jobs);*/
    /*int lmc = ceil(alfa*numJobs);*/
    /*[>job* cand = extraiJob(&listaCandidatos,retornaIndice(lmc));<]*/
    /*int ind;*/
    /*job* excedente = NULL;*/
    /*int numExc = 0;*/
    /*do{*/
       /*job* jobCandidato = extraiJob(&listaCandidatos,retornaIndice(lmc)); */
       /*if(maquinas[itMaq].custo * jobCandidato->tempo + calculaCusto(maquinas) <= restricao){*/
        /*if(jobCandidato->tempo+maquinas[itMaq].tempoTotal <= SpanMin){*/
            /*inserirJob(jobCandidato,itMaq,maquinas);*/
        /*}else{*/
            /*reservaCandidato(&excedente,&jobCandidato,&numExc);*/
        /*}*/
       /*}else{*/
           /*if(listaCandidatos==NULL){*/
            /*reservaCandidato(&listaCandidatos,&jobCandidato,&numExc);*/
           /*}*/
       /*}*/
    /*}while(listaCandidatos != NULL);*/
/*}*/

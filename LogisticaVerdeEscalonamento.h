#ifndef LOGISTICAVERDEESCALONAMENTO_H


int restricao;
    typedef struct i job;
    struct i{
        int id;
        job *job;
        int tempo;
    };


    typedef struct {
        int id;
        job *job;
        int  custo;
        int tempoTotal;
    }maquina;
    int retNumMaq();
    int calculaMakespan(maquina* naquinas);
    void makeSpanMin(maquina* maquinas,job* jobs,int itJob);
    void makeSpanMin2(maquina* maquinas,job* jobs,int itJob);
//    void construtivo(maquina* maquinas, job* jobs, double alfa);
    void construtivo(maquina* maquinas,job* jobs,double alfa, int tempoTotal,int SpanMin, int custoMenor,int divisor);
    job* preparaJobs(const job* jobs);
    void construtivo1(maquina* maquinas,const job* jobs,double alfa, int tempoTotal,int SpanMin, int custoMenor,int divisor,int flagCusto);

    job* extraiJob(job** jobs, int indice);
    int retornaIndice(int lmc);
//    void makeSpanMin(maquina* maquinas,int* tempoJob,int tempoTotalJobs,double taxa,int itJob);
    maquina* criaMaquinas(int *custoMaquina);
    int troca(maquina* maquina1,maquina* maquina2);
    void otimizacao2(maquina* maquinas);
    int viabiliza(maquina* maquinas,job* jobs,int* itJob,int itMaq);
    void  verificaTodosOsJobs2(int tempoTotal,maquina* saida);
    int otimizacao(maquina* maquinas,int ind,int i);
//    int inserirJob(job** jobs,int m,maquina *maquinas);
    int inserirJob(job* jobCandidato,int m,maquina *maquinas);
    maquina* randReativo(maquina* maquinas, job* jobs, char metrica);
    int inserirJobSimples(job* jobs,int m,maquina *maquinas);
    static int cmpInt(const void *p1, const void *p2);
    static int cmpTempoJob(const void *p1, const void *p2);
    //void heuristica(maquina* maquinas, job* jobs);
    static int cmpTempoTotal(const void *p1, const void *p2);
    static int cmpcustomaquina(const void *p1, const void *p2);
    int tempoJobs(int* tempoJobs);
    void leInstancia(maquina** maquinas,job** jobs, char* nomeInstancia );
    void PrintSaida(maquina* saida);
    void refinaCusto(maquina* maquinas);
    int calculaCusto(maquina* saida);
    int  randomico(maquina* maquinas,job* jobs,float alfa);
    void viabilizaConstrutivo(maquina* maquinas, job** listaCandidatos);
//    void preparaJobs(job** job);
    //job* extraiJob(job** jobs,int indice);
//    void printLista(job* jobs);
  int tempoTotalJobs(job* jobs);
    int calculaCustoTotal(maquina* maquinas);
    void imprimeLista(job* jobs);
    void verificaTodosOsJobs(int , maquina*);
    maquina* copiaMaq(maquina* maquinas);
    int calculaMakespan(maquina* maquinas);
    void destroiSolucao(maquina* maquinas);
    void selecionaConstrutivo(maquina** maquinas, job* jobs,double alfa, int tempoTotal,int SpanMin);
    void selecionaMetrica(maquina** maquinas,job* jobs,double alfa, int tempoTotal,int spanMinTeo, int proporcao, char metrica);

    void construtivo2(maquina* maquinas,const job* jobs,double alfa, int tempoTotal,int SpanMin,int taxa,int flag);
#define LOGISTICAVERDEESCALONAMENTO_H

#endif /* LOGISTICAVERDEESCALONAMENTO_H */

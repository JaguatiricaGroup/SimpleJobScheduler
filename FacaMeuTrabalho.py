
import random
import os
import sys,shutil
import time
import subprocess
from subprocess import Popen,PIPE, STDOUT
import glob
import pandas as pd

def geraTabela(makespans,tempos,nomesInstancias,custos,restricoes,L,metrica):
    import pandas as pd
    df = pd.DataFrame({"a":nomesInstancias,"b":makespans, "c":tempos, "d":custos, "e":restricoes ,"f":numJobs , "g":numMaq })
    with open("saida/tabelaResultados"+"metrica:"+str(metrica)+"Execucao:"+str(L)+".tex", "w") as f:
        f.write("\\begin{longtable}{" + " | ".join(["c"] * len(df.columns)) + "}\n")
        f.write("Instância  & Makespan(s) & tempo Execução(s) & custoTotal & Restricao & NumJobs & NumMaq\\\\\n")
        #print("\\begin{tabular}{" + " | ".join(["c"] * len(df.columns)) + "}\n")
        for i, row in df.iterrows():
            f.write(" & ".join([str(x) for x in row.values]) + " \\\\\n")
      #      print((" & ".join([str(x) for x in row.values]) + " \\\\\n"))
        f.write("\\end{longtable}")
        f.close()

random.seed(7)
subprocess.run('make')
# metrica = sys.argv[2]
iteracoes = 30
r = 0
for l in range(4):
    for j in range(iteracoes):
            nomesInstancias= list()
            makespans = list()
            tempos = list()
            restricoes = list()
            custos = list()
            numJobs = list()
            numMaq = list()
            k=0
            arq = open('saida/logExe'+str(j)+'metrica'+str(l),'w')
            for filename in glob.iglob('instancias/**/*.txt', recursive=True):
                    k=k+1
                    # print(filename+': '+str(k))

                    #print(sys.argv[1])
                    # print([sys.argv[1],filename,0,random.randint(0, 100000)])
                    start = time.time()
                    saidaComando = subprocess.check_output([sys.argv[1],filename,str(l),str(random.randint(0, 100000))])
                    elapsed = time.time() - start
                    caminho= ''
                    # print(saidaComando)
                    for i in range(0, len(filename)):
                            char = filename[i+11]
                            if char != "/":
                                    caminho+=char
                            else:
                                    break
                    # print(caminho)
                    # os.makedirs('saida/'+caminho+'/',exist_ok=True)
                    # arq = open('saida/log'+str(k),'w')
                    saida = saidaComando.decode("utf-8")
                    # print(saida)
                    info = list()
                    i=0
                    for linha in saida.split('\n') :
                            if linha :
                            # print(linha)
                            # print(i)
                                    info.append(int(linha[3:]))
                    if(info[0]>info[1]):
                            print(filename)
                            print("nao funcionou")
                            print(str(info[0])+"  ")
                            info[0] *= -1
                            print(str(info[1]))
                            info[1] *= -1
                            exit(6)
                    # arq.write("makespan: "+ str(info[2])+"\n" +" tempo: "+ str(elapsed))
                    # os.system(sys.argv[1]+ " " +filename)
                    # print(elapsed)
                    arq.write(str([sys.argv[1],filename,0,random.randint(0, 100000)])+'\n'+saida)
                    custos.append(info[0])
                    restricoes.append(info[1])
                    nomesInstancias.append(filename[12:].split('/')[1])
                    makespans.append(info[2])
                    numJobs.append(info[3])
                    numMaq.append(info[4])
                    tempos.append(elapsed)
                    r+=k
                    subprocess.call("clear")
                    print("Metrica: "+str(l)+"\n"+"Execucao: "+str(j)+"\n"+'Instancia: '+str(k))
            geraTabela(makespans,tempos,nomesInstancias,custos,restricoes,j,l)

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define HV 2147483647

int C;
int N;

int T,TF;
int TPSA,TPLL;
int GA,GC,I;

int *TPS = NULL;
int *CDA = NULL;

int IA,ITO,J,NT,SPS,TC,STC,STO;

float random();
void inicializar_variables();
void simular();
void primera_rama();
void segunda_rama();
void tercera_rama();
int GENERAR_IA();
int GENERAR_TC();

int PRIMER_LIBRE(){
    J = 0;
    int K;

    for (K=1; K<N; K++){
        if (TPS[K]>TPS[J]) {
            J = K;
        }
    }

    return J;
}

int MIN_TPS(){
    J = 0;
    int K;

    for (K=1; K<N; K++){
        if (TPS[K]<TPS[J]) {
            J = K;
        }
    }

    return J;
}


void primera_rama() {
    SPS = SPS + (TPLL-T)*(GA+GC);
    T = TPLL;
    IA = GENERAR_IA();
    TPLL = TPLL + IA;
    GC = GC + 1;

    if (GC == 1 && TPSA == HV) {

        TC = GENERAR_TC();
        TPSA = T + TC;
        GA = GA + 1;
        GC = 0;
        STC = STC + TC;
        STO = STO + (T-ITO);

    } else {

        if (GA < N + 1 && GC > C) {

            TC = GENERAR_TC();
            I = PRIMER_LIBRE();
            TPS[I] = T + TC;
            GA = GA + 1;
            GC = GC - 1;
            STC = STC + TC;

        }
    }
}

void segunda_rama() {
    SPS = SPS + (TPS[I] - T)*(GA + GC);
    T = TPS[I];
    GA = GA - 1;

    if (GC > C) {

        TC = GENERAR_TC();
        TPS[I] = T + TC;
        GC = GC - 1;
        GA = GA + 1;
        STC = STC + TC;

    } else {

        TPS[I] = HV;

    }

    CDA[I] = CDA[I] + 1;
    NT = NT + 1;
}

void tercera_rama() {
    SPS = SPS + (TPSA - T)*(GA + GC);
    T = TPSA;
    GA = GA - 1;

    if (GC > 0) {

        TC = GENERAR_TC();
        TPSA = T + TC;
        GC = GC - 1;
        GA = GA + 1;
        STC = STC + TC;

    } else {

        TPSA = HV;
        ITO = T;

    }

    NT = NT + 1;
}


void imprimir_resultados(){
    float PTO,PTE,PDA;

    PTO = (STO*100)/T;
    PTE = (SPS-STC)/NT;

    printf("PTO = %.2f%%\n", PTO);
    printf("PTE = %.2fm\n", PTE);

    for (I=0; I<N; I++){
        PDA = (CDA[I]*100)/NT;
        printf("PDA[%d] = %.2f%%\n", I+1, PDA);
    }
}

void inicializar_variables() {
    int I;
    T = 0;
    TF = 10000;
    TPLL = 0;
    TPSA = HV;
    for (I=0; I<N; I++) {
        TPS[I] = HV;
    }
    GC = 0;
    GA = 0;

    SPS = 0;
    STC = 0;
    STO = 0;
    ITO = 0;
    NT = 0;
    for (I=0; I<N; I++) {
        CDA[I] = 0;
    }
}

float random() {
    return (float) rand() / (float) RAND_MAX;
}

void simular() {

    A:

    I = MIN_TPS();
    if (TPLL <= TPSA) {

        if (TPLL <= TPS[I]) {

           primera_rama();

        } else {

           segunda_rama();

        }
    } else {

        if (TPS[I] <= TPSA) {

           segunda_rama();

        } else {

           tercera_rama();

        }
    }

    if (T <= TF) goto A;

    imprimir_resultados();
}

int GENERAR_IA() {
    return 15;
}

int GENERAR_TC() {
    return 16;
}

int main(){


    for (C=1; C<5; C++) {
        for (N=0; N<3; N++) {

            free(TPS);
            free(CDA);
            TPS = (int*)calloc(N,sizeof(int));
            CDA = (int*)calloc(N,sizeof(int));

            printf("C=%d N=%d\n", C, N);
            inicializar_variables();
            simular();
        }
    }

    return 0;
}

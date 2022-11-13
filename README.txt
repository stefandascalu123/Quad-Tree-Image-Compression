    Dascalu Stefan Teodor, 314CC
    Tema 2 - Quad Tree

Implementare functia CreateTree

    Functia construieste arborele quaternar pe baza
    unei matrice de pixeli care constituie imaginea initiala
    in mod recursiv. Salveaza in nodul curent marime
    zonei (numarul de pixeli) si apoi calculeaza
    culoarea medie pentru fiecare canal RGB urmand
    sa se calculeze valoarea mean. Daca mean este
    mai mic sau egal cu factorul primit ca argument
    zona nu se mai imparte in 4 si se salveaza culorile
    medii in nodul curent. In caz contrar, functia se apeleaza
    recursiv in urmatoarele 4 zone.

Implementare functia CreateArray
T
    Functia constuieste vectorul de noduri QuadtreeNode, salveaza
    memoria din fiecare nod al arborelui quaternar in zona vectorului.
    Daca nodul este o frunza(adica nu este impartit), variabila care
    calculeaza nr de culori al arborului este incrementat iar indicii
    urmatoarelor 4 zone o sa fie setat -1. In caz contrar, se salveaza
    indicii urmatoare 4 zone cu indexul din vector corespunzator si
    se apeleaza recursiv functia.

Implementare functia ReconstructTree

    Functia recursiva reconstruieste arborele quaternar pe baza unui vector
    de compresie. Se aloca memoria pt un nod si i se atribuia marimea in pixeli,
    daca nodul nu este impartit se salveaza culorile RGB, in caz contrat se apeleaza
    recursiv functia pt urmatorare 4 noduri alaturi de indicii in vector.

Implementare functia ReconstructImage

    Functia recursiva salveaza latimea zonei si verifica daca nodul curent este frunza,
    daca da, in matricea imaaginii se salveaza valorile componentelor RGB. Daca nu,
    se apeleaza recursiv pt nodurile copil.

Implementare functii MirrorH si MirrorV

    Ambele functii schimba legaturile unui nod in arborele quaternar pt
    realizarea oglindirii pe orizontala sau pe verticala.

Implementare functia FreeTree

    Functia elibereaza recursiv memoria dintr-un arbore quaternar

Implementare main

    Main-ul verifica valoarea lui argv[1], si executa algoritmul pt cerinta corespunzatoare.

Implementare compresie

    Se deschid fisierele input si output, primite prin argumente in linia de comanda, apoi
    se citeste descria imaaginii PPM, si se salveaza lungimea si latimea, se aloca memorie
    pt matricea de pixeli si se citest componentele rand pe rand. Se apeleaza functia
    CreateTree, se creeaza compresia si arborele quaternar. Apoi se apeleaza functia CreateArray,
    pt realizare vectorului de noduri. Apoi se afiseaza elementele pe rand in fisierul output.

Implementare decompresie

    Analog se deschid fisierele, se aloca memorie pt vectorul de noduri si se citeste din
    fisierul output, se apeleaza ReconstructTree, se reconstruieste arborele. Apoi
    se apeleaza ReconstructImage, reconstruindu-se imaginea propriu zisa. Apoi,
    se afiseaza in fisierul outpui descrierea citita si rand pe rand elementele
    matricei de pixeli.

Implementare Mirror

    Se realizaza compresia imaginii, se apeleaza functia Mirror corespunzatoare si se
    inlocuiesc elementele matricei de pixeli,Apoi,
    se afiseaza in fisierul outpui descrierea citita si rand pe rand elementele
    matricei de pixeli.


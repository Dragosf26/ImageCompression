Acest programul are functionalitatea de a oferi date si de a compresa o poza de tip PPM. In realizarea acestui program au
fost folosite urmatoarele functii:
-calculateMean: functia care, pentru fiecare submatrice patratica din poza, calculeaza mai intai media intensitatii culorilor
rosu, verde si albastru, apoi returneaza scorul de compresie
-createTree: functia care, daca scorul de compresie returnat de functia precedenta este mai mare decat toleranta, creeaza un
nod intern, iar daca scorul este mai mic sau egal decat toleranta, ii atribuie nodului intensitatea culorilor rosu, verde si
albastru
-maxDepth: functia care returneaza inaltimea maxima a arborelui cuaternar
-minDepth: functia care returneaza inaltimea minima a arborelui cuaternar
-printLevel: functia care scrie pe nivel in fisierul binar datele arborelui, incluzand tipul nodului, precum si intensitatea
culorilor rosu, verde si albastru
-levelOrderTraversal: functia care apeleaza functia printLevel pentru fiecare nivel din arborele cuaternar
-eliberare_memorie_matrice: functia care elibereaza memoria alocata pentru matrice
-eliberare_memorie_arbore: functia care elibereaza memoria alocata pentru arbore
-citire: functia care citeste din fisier, precum si aloca dinamic memorie pentru matrice
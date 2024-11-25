Implementarea codului
In main imi initializez structura pt sfl si pt cele alocate.Apoi in cadrul unui while citesc comenzile de la tastatura cu tot cu parametrii.
Functia init_heap:
In ea intializez structura sfl si caut ce numar de blocuri de poate creea cu numarul de bytes introdus dupa care calculez adresele la care 
se vor afla blocurile prin adunare cu marimea bloclui si apoi dupa ce s au terminat blockurile inmultesc cu 2 pentru a calcula urmatoarele adrese.Calculez de asemenea
memoria totala si memoria libera.De asemenea o data cu init_heap initializez si strucutra de alocate alegand sa lucrez tot cu un vector de liste.
Functia Malloc:
Incrementez numarul de malloc calls si caut apoi adresa care se doreste sa fie alocata,daca nu necesita fragmentare blokul se adauga acesta in vectorul de alocate,iar daca exista deja o lista corespunzatoare il adaug acolo daca nu realoc vectorul de liste.
Daca necesita fragmentare caut adresa apoi fragmentez blocul adaugand in structura de alocate blockul respectiv daca exista deja o lista, daca nu creez,iar apoi restul de bytes ii adaug in sfl pe acelasi principiu daca exista lista necesara,daca nu creez.
Functia de Free:
Intai verific daca adresa unde s a cerut eliberarea este alocata daca nu afisez mesaj de eroare daca da atunci caut adresa,eliberez blockul respectiv si il mult in sfl daca exista o lista necesara daca nu creez.
Functia Dump_Memory:
Afisez tot ceea ce se cere,pt slf afisarea se face dupa list_bytes si fiecare linie dupa adresa,iar alocatele le mut intr o simpla lista pe care o sortez dupa adrese si afisez nodurile respective.
Functia de Write:
Verific daca adresa unde s a cerut scrierea este alocata,daca nu afisez un mesaj de eroare si de asemenea verific si daca este cazul de scriere in mai multe blockuri consecutive daca si urmatoarele sunt suficient de alocate.
Apoi verific pe ce caz ma aflu daca trebuie sa scriu doar intr un singur block verific daca a fost deja scris pt a suprascrie daca nu,doar copiez mesajul la adresa respectiva.
Daca se necesita scrierea la mai multe blockuri scriu intai cat este necesar in primul block salvez restul sirului pe care il am de scris si il scriu in urmatorul block de memorie.
Functia Read:
Verifica daca adresa unde se doreste citirea a fost alocata si apoi daca sunt suficiente blocuri de citit pt a nu citi ceva "NULL".
In cazul in care citirea se fac doar dintr un block copiez sirul de acolo intr un sir auxiliar pe care apoi il afisez.Daca este necesara citirea din mai multe blockuri
citesc pe rand cate "block_size" bytes pana ajung sa fie nevoie sa citesc doar dintr un block.Apoi ce am citit se baga intr un sir pe care il afisez.
Functia Destroy_Heap:
Se plimba 
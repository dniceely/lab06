/* First, the standard lib includes, alphabetically ordered */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "abb.h" /* TAD abb */


void print_help(char *program_name) {
    /* Print the usage help of this program. */
    printf("Usage: %s <input file path>\n\n",
           program_name);
}

char *parse_filepath(int argc, char *argv[]) {
    /* Parse the filepath given by command line argument. */
    char *result = NULL;

    if (argc < 2) {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    result = argv[1];

    return (result);
}

abb abb_from_file(const char *filepath) {
    FILE *file = NULL;
    abb read_tree;

    read_tree = abb_empty();
    file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "File does not exist.\n");
        exit(EXIT_FAILURE);
    }
    unsigned int i = 0u;
    unsigned int size = 0u;
    int res = 0;
    res = fscanf(file, " %u ", &size);
    if (res != 1) {
        fprintf(stderr, "Invalid format.\n");
        exit(EXIT_FAILURE);
    }
    while (i < size) {
        abb_elem elem;
        res = fscanf(file," %d ", &(elem));
        if (res != 1) {
            fprintf(stderr, "Invalid array.\n");
            exit(EXIT_FAILURE);
        }
        read_tree = abb_add(read_tree, elem);

       ++i;
    }
    fclose(file);
    return read_tree;
}

int main(int argc, char *argv[]) {
    char *filepath = NULL;

    /* parse the filepath given in command line arguments */
    filepath = parse_filepath(argc, argv);

    /* parse the file to obtain an abb with the elements */
    abb tree = abb_from_file(filepath);

    /*dumping the tree*/
    abb_dump(tree);
    if (!abb_is_empty(tree)) {
        printf("\n");
        printf("raiz: %d\n minimo: %d\n maximo: %d\n", abb_root(tree),
                                                       abb_min(tree),
                                                       abb_max(tree));
    } else {
        printf("\nÁrbol vacío\n");
    }

    tree = abb_destroy(tree);
    /*
     * Modificar e implementar con un ciclo una interfaz que permita al usuario
     * realizar una de las siguientes operaciones en cada iteración:
     *
     * 1 ........ Mostrar árbol por pantalla
     * 2 ........ Agregar un elemento
     * 3 ........ Eliminar un elemento
     * 4 ........ Chequear existencia de elemento
     * 5 ........ Mostrar longitud del árbol
     * 6 ........ Mostrar raiz, máximo y mínimo del árbol
     * 7 ........ Salir
     *
     * Se debe solicitar un número de entrada para realizar una de las acciones.
     *
     * Para las opciones 2, 3 y 4 se le deberá pedir al usuario que ingrese el
     * elemento a agregar, eliminar o chequear respectivamente.
     *
     * Al salir debe liberarse toda la memoria utilizada.
     *
     */

    // la idea de la interfaz es que el usuario haga un arbol como quiera? entonces no necesito recuperar el arbol anterior creo?

    int op = 0;
    abb new_tree;
    new_tree = abb_empty();
    abb_elem elem;
    bool check = true;
    while (check)
    {
    printf("Que operación quiere realizar?\n");
    printf("1- Mostrar árbol por pantalla\n2- Agregar un elemento\n3- Eliminar un elemento\n4- Chequear existencia de elemento\n5- Mostrar longitud del árbol\n6-Mostar raíz, máximo y minimo del árbol\n7- Salir\n\n");
    scanf("%d", &op);
    // tuve que buscar switch en el manual porque con un while se complicaba el funcionamiento
    switch (op)
    {
        case 1:
            if (abb_is_empty(new_tree))
            {
                printf("El árbol está vacío\n");
            }
            else
            {
                abb_dump(new_tree);
            }
            break;
            
        case 2:
            printf("Ingrese un número para añadir al árbol\n");
            scanf("%d", &elem);
            new_tree = abb_add(new_tree,elem);
            break;

        case 3:
            printf("Ingrese un número para eliminar del árbol\n");
            scanf("%d", &elem);
            if (abb_exists(new_tree,elem))
            {
                new_tree = abb_remove(new_tree,elem);
            }
            else
            {
                printf("El número no está en el árbol\n");
            }
            break;

        case 4:
            printf("Ingrese un número para chequear su existencia\n");
            scanf("%d", &elem);
            if (abb_exists(new_tree,elem))
            {
                printf("El número está en el árbol\n");
            }
            else
            {
                printf("El número no está en el árbol\n");
            }
            break;

        case 5:
            printf("La longitud del árbol es: %u\n", abb_length(new_tree));
            break;

        case 6:
            if (!abb_is_empty(new_tree)) {
            printf("\n");
            printf("raiz: %d\n minimo: %d\n maximo: %d\n", abb_root(new_tree), abb_min(new_tree), abb_max(new_tree));
    } else {
        printf("\nÁrbol vacío\n");
    }
        break;

        case 7:
            printf("Saliendo del programa de árbol...\n");
            new_tree = abb_destroy(new_tree);
            check = false;
            break;

            default:
            printf("Opcíon invalida\n");
    }
    }

    return (EXIT_SUCCESS);
}

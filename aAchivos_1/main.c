
/* 
 * File:   main.c
 * Author: def
 *
 * Created on 8 de octubre de 2019, 17:42
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

char * gets(char *);

typedef struct {
    int codigo; //primarykey
    char descripcion[41];
    float precio;
} tproducto;

void continuar() {
    printf("Presione una tecla para continuar\n\n");
    getch();
}

/* funcion para crear el archivo */
void crear() {
    FILE *archivo, *copiaSeguridad;
    char resp = 'n';

    //leer archivo binario
    if (fopen("productos.dat", "rb") == NULL) {//el archivo no existe
        printf("Error: %d, %s\n", sys_nerr, sys_errlist[sys_nerr]);
        archivo = fopen("productos.dat", "wb"); //leer archivo binario
        printf("fichero creado");
        //exit(1); //error
    } else {
        do {
            fflush(stdin);
            printf("Este fichero existe ¿Desea sobrescribirlo? (s/n) ");
            resp = tolower(getchar());

            if (resp == 's') {
                archivo = fopen("productos.dat", "wb");
            } else {
                archivo = fopen("productos.dat", "rb");
            }
        } while (resp != 's' && resp != 'n');
    }


    fclose(archivo);
    continuar();
}

/*cargar datos de 1 estructura dentro del archivo*/
void cargar() {
    tproducto producto;
    FILE *archivo;
    archivo = fopen("productos.dat", "ab"); //abrir para añadir al archivo binario
    if (archivo == NULL) {
        exit(1);
    }
    /*pedir los datos por pantalla*/
    printf("Ingrese el codigo del producto:");
    scanf("%i", producto.codigo); //&producto.codigo
    fflush(stdin);
    printf("Ingrese el nombre del producto:");
    gets(producto.descripcion);
    printf("Ingrese precio:");
    scanf("%f", &producto.precio);

    /*pasar los datos al archivo */
    fwrite(&producto, sizeof (tproducto), 1, archivo);
    fclose(archivo);
    continuar();
}

/*mostar una lista de los productos*/
void listado() {
    tproducto producto;
    FILE *archivo;
    archivo = fopen("productos.dat", "rb"); //abrir el archivo en modo lectura binaria
    if (archivo == NULL) {
        exit(1);
    }

    fread(&producto, sizeof (tproducto), 1, archivo); //leo los datos
    printf(" Codigo Descripcion Precio \n-----------------------------------\n");
    //mientras tenga datos el archivo ejecuta
    while (!feof(archivo)) {
        printf("   %i       %s       %0.2f\n", producto.codigo, producto.descripcion, producto.precio);
        fread(&producto, sizeof (tproducto), 1, archivo); //mientrar tenga datos sigue leyendo
    }

    fclose(archivo);
    continuar();
}

void consulta() {
    int cod, existe = 0;
    tproducto producto;
    FILE *archivo;
    archivo = fopen("productos.dat", "rb"); //lectura binaria
    if (archivo == NULL) {
        exit(1);
    }

    printf("Ingrese el codigo de producto a consultar:");
    scanf("%i", &cod);

    fread(&producto, sizeof (tproducto), 1, archivo); //leer la primera vez para comprobar los datos
    //mientras tenga datos el archivo ejecuta
    while (!feof(archivo)) {
        if (cod == producto.codigo) {
            printf("%i %s %0.2f\n", producto.codigo, producto.descripcion, producto.precio);
            existe = 1;
            break;
        }
        fread(&producto, sizeof (tproducto), 1, archivo);
    }
    if (existe == 0) {
        printf("No existe un producto con dicho codigo\n");
    }

    fclose(archivo);
    continuar();
}

void modificacion() {
    int cod, existe = 0;
    tproducto producto;
    FILE *archivo;
    archivo = fopen("productos.dat", "r+b"); //leer y escribir el archivo tiene que existir
    if (archivo == NULL) {
        exit(1);
    }

    printf("Ingrese el codigo de producto a modificar:");
    scanf("%i", &cod);

    fread(&producto, sizeof (tproducto), 1, archivo);
    //mientras tenga datos el archivo ejecuta
    while (!feof(archivo)) {
        //si coincide el codigo indicado y el del archivo
        if (cod == producto.codigo) {
            printf("datos antiguos: %i %s %0.2f\n", producto.codigo, producto.descripcion, producto.precio);
            printf("Ingrese nuevo precio:");
            scanf("%f", &producto.precio);

            //ftell-> función permite obtener la posición en el fichero del puntero de L/E.
            int pos = ftell(archivo) - sizeof (tproducto);

            /*Esta función permite fijar la posición donde se realizará la siguiente operación de L/E.
             * SEEK_SET Hace referencia a la primera posición en el fichero.
               SEEK_CUR Hace referencia a la posición actual del puntero de L/E.
               SEEK_END Hace referencia a la última posición en el fichero.*/
            fseek(archivo, pos, SEEK_SET);

            fwrite(&producto, sizeof (tproducto), 1, archivo);
            printf("Se modifico el precio para dicho producto.\n");
            existe = 1;
            break;
        }
        fread(&producto, sizeof (tproducto), 1, archivo);
    }
    if (existe == 0) {
        printf("No existe un producto con dicho codigo\n");
    }

    fclose(archivo);
    continuar();
}

void copiaDeSeguridad() {
    FILE *copiaseguridad = NULL, *archivo = NULL;

    int car = 0;
    if ((archivo = fopen("productos.dat", "rb")) == NULL || (copiaseguridad = fopen("copiaseguridad.dat", "wb")) == NULL) {
        printf("No se puede realizar la copia\n");
        exit(1);
    }
    // Copiar
    while (!ferror(archivo) && !feof(archivo) && !ferror(copiaseguridad)) {
        car = fgetc(archivo);
        fputc(car, copiaseguridad);
    }
    // Verificar si la copia se hizo con éxito
    if (ferror(archivo) || ferror(copiaseguridad)) {
        perror("Error durante la copia");
    } else {
        printf("creado correctamente\n");
    }

    fclose(archivo);
    fclose(copiaseguridad);

    continuar();
}

int main() {
    int opcion;
    do {
        printf("1 - Crear un archivo binario llamado \"productos.dat\"\n");
        printf("2 - Intruducir registros de tipo tproducto\n");
        printf("3 - Listado completo de productos.\n");
        printf("4 - Consulta de un producto por su codigo.\n");
        printf("5 - Modificacion del precio de un producto. \n");
        printf("6 - Crear copia de seguridad del archivo\n");
        printf("7 - SAlir\n\n");
        printf("Ingrese su opcion:");
        scanf("%i", &opcion);
        switch (opcion) {
            case 1:crear();
                break;
            case 2:cargar();
                break;
            case 3:listado();
                break;
            case 4:consulta();
                break;
            case 5:modificacion();
                break;
            case 6:copiaDeSeguridad();
                break;
        }
    } while (opcion != 7);
    return 0;
}
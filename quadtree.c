#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

// structura pt vectorul de noduri
typedef struct QuadtreeNode
{
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;

} __attribute__((packed)) QuadtreeNode;

// structura pentru nodul arborului quaternar
typedef struct QuadTree
{
    unsigned char blue, green, red;
    uint32_t area;
    struct QuadTree *top_left, *top_right;
    struct QuadTree *bottom_left, *bottom_right;

} QuadTree;

// structura pt elementele matricei imaginii
typedef struct Image
{
    unsigned char blue, green, red;
} Image;

// functie care returneaza un nod al arborului gol
QuadTree *getNewNode()
{
    QuadTree *newNode = (QuadTree *)malloc(sizeof(QuadTree));
    newNode->area = 0;
    newNode->top_left = NULL;
    newNode->top_right = NULL;
    newNode->bottom_right = NULL;
    newNode->bottom_left = NULL;
    return newNode;
}

// functia recursiva care creeaza arborele quaternar de compresie pe baza matricei imaginii
void CreateTree(QuadTree **quad, int x, int y, int size, int factor, Image **image, int *nr_nodes)
{
    long long red = 0, green = 0, blue = 0, mean = 0;
    (*quad) = getNewNode();
    (*nr_nodes)++;
    (*quad)->area = size * size;

    for (int i = x; i < x + size; i++)
    {
        for (int j = y; j < y + size; j++)
        {
            red += image[i][j].red;
            green += image[i][j].green;
            blue += image[i][j].blue;
        }
    }

    red = red / (size * size);
    green = green / (size * size);
    blue = blue / (size * size);

    for (int i = x; i < x + size; i++)
    {
        for (int j = y; j < y + size; j++)
        {
            mean += (red - image[i][j].red) * (red - image[i][j].red) + (green - image[i][j].green) * (green - image[i][j].green) + (blue - image[i][j].blue) * (blue - image[i][j].blue);
        }
    }
    mean = mean / (3 * size * size);

    if (mean <= factor)
    {

        (*quad)->red = blue;
        (*quad)->green = green;
        (*quad)->blue = red;
    }
    else
    {

        (*quad)->red = 0;
        (*quad)->green = 0;
        (*quad)->blue = 0;

        CreateTree(&(*quad)->top_left, x, y, size / 2, factor, image, nr_nodes);
        CreateTree(&(*quad)->top_right, x, y + size / 2, size / 2, factor, image, nr_nodes);
        CreateTree(&(*quad)->bottom_right, x + size / 2, y + size / 2, size / 2, factor, image, nr_nodes);
        CreateTree(&(*quad)->bottom_left, x + size / 2, y, size / 2, factor, image, nr_nodes);
    }
}

// functie care verifica daca un nod al arborului este o frunza
int CheckLeaf(QuadTree *quad)
{
    if (quad->bottom_left == NULL && quad->bottom_right == NULL)
    {
        if (quad->top_left == NULL && quad->top_right == NULL)
        {
            return 1;
        }
    }
    return 0;
}

// functia recursiva care creeaza vectorul de noduri pe baza unui arbore quaternar
void CreateArray(QuadTree *quad, QuadtreeNode *nodes, int *len_cur, int *nr_colors)
{
    nodes[*len_cur].area = quad->area;
    nodes[*len_cur].red = quad->red;
    nodes[*len_cur].green = quad->green;
    nodes[*len_cur].blue = quad->blue;

    if (CheckLeaf(quad) == 1)
    {
        (*nr_colors)++;
        nodes[*len_cur].bottom_left = -1;
        nodes[*len_cur].bottom_right = -1;
        nodes[*len_cur].top_left = -1;
        nodes[*len_cur].top_right = -1;
    }
    else
    {
        int current = (*len_cur);
        nodes[current].top_left = (*len_cur) + 1;
        (*len_cur)++;
        CreateArray(quad->top_left, nodes, len_cur, nr_colors);

        nodes[current].top_right = (*len_cur) + 1;
        (*len_cur)++;
        CreateArray(quad->top_right, nodes, len_cur, nr_colors);

        nodes[current].bottom_right = (*len_cur) + 1;
        (*len_cur)++;
        CreateArray(quad->bottom_right, nodes, len_cur, nr_colors);

        nodes[current].bottom_left = (*len_cur) + 1;
        (*len_cur)++;
        CreateArray(quad->bottom_left, nodes, len_cur, nr_colors);
    }
}

// functie recursiva care creeaza un arbore quaternar pe baza unui vector de noduri
void ReconstructTree(QuadtreeNode *nodes, QuadTree **quad, int len_cur)
{
    (*quad) = getNewNode();
    (*quad)->area = nodes[len_cur].area;

    if (nodes[len_cur].bottom_left == -1 && nodes[len_cur].bottom_right == -1 && nodes[len_cur].top_left == -1 && nodes[len_cur].top_right == -1)
    {
        (*quad)->red = nodes[len_cur].blue;
        (*quad)->green = nodes[len_cur].green;
        (*quad)->blue = nodes[len_cur].red;
    }
    else
    {
        ReconstructTree(nodes, &(*quad)->top_left, nodes[len_cur].top_left);
        ReconstructTree(nodes, &(*quad)->top_right, nodes[len_cur].top_right);
        ReconstructTree(nodes, &(*quad)->bottom_right, nodes[len_cur].bottom_right);
        ReconstructTree(nodes, &(*quad)->bottom_left, nodes[len_cur].bottom_left);
    }
}

// functie recursiva care creeaza o imagine pe baza unui arbore cuaternar
void ReconstructImage(Image **image, QuadTree *quad, int x, int y)
{
    int size = sqrt(quad->area);
    if (CheckLeaf(quad) == 1)
    {
        for (int i = x; i < x + size; i++)
        {
            for (int j = y; j < y + size; j++)
            {
                image[i][j].red = quad->red;
                image[i][j].green = quad->green;
                image[i][j].blue = quad->blue;
            }
        }
    }
    else
    {
        ReconstructImage(image, quad->top_left, x, y);
        ReconstructImage(image, quad->top_right, x, y + size / 2);
        ReconstructImage(image, quad->bottom_right, x + size / 2, y + size / 2);
        ReconstructImage(image, quad->bottom_left, x + size / 2, y);
    }
}

// functie recursiva care schimba legaturile unui arbore quaternar
// pt a oglindi elementele 1 cu 2 si 3 cu 4
void MirrorH(QuadTree **quad)
{
    if (CheckLeaf((*quad)) == 0)
    {
        MirrorH(&(*quad)->top_left);
        MirrorH(&(*quad)->top_right);
        MirrorH(&(*quad)->bottom_right);
        MirrorH(&(*quad)->bottom_left);

        QuadTree *aux = (*quad)->top_left;

        (*quad)->top_left = (*quad)->top_right;
        (*quad)->top_right = aux;

        aux = (*quad)->bottom_left;

        (*quad)->bottom_left = (*quad)->bottom_right;
        (*quad)->bottom_right = aux;
    }
    else
    {

        long long aux = (*quad)->red;
        (*quad)->red = (*quad)->blue;
        (*quad)->blue = aux;
    }
}

// functie recursiva care schimba legaturile unui arbore quaternar
// pt a oglindi elementele 1 cu 4 si 2 cu 3
void MirrorV(QuadTree **quad)
{
    if (CheckLeaf((*quad)) == 0)
    {
        MirrorV(&(*quad)->top_left);
        MirrorV(&(*quad)->top_right);
        MirrorV(&(*quad)->bottom_right);
        MirrorV(&(*quad)->bottom_left);

        QuadTree *aux = (*quad)->top_left;

        (*quad)->top_left = (*quad)->bottom_left;
        (*quad)->bottom_left = aux;

        aux = (*quad)->top_right;

        (*quad)->top_right = (*quad)->bottom_right;
        (*quad)->bottom_right = aux;
    }
    else
    {

        long long aux = (*quad)->red;
        (*quad)->red = (*quad)->blue;
        (*quad)->blue = aux;
    }
}

// functie recursiva care elibereaza memoria alocata pt arbore
void FreeTree(QuadTree *quad)
{
    if (CheckLeaf(quad) == 0)
    {
        FreeTree(quad->bottom_left);
        FreeTree(quad->bottom_right);
        FreeTree(quad->top_left);
        FreeTree(quad->top_right);
        free(quad);
    }
    else
    {
        free(quad);
    }
}

void main(int argc, char *argv[])
{

    if (strcmp(argv[1], "-c") == 0)
    {
        long long factor = atoi(argv[2]);
        FILE *input = fopen(argv[3], "rb");
        FILE *output = fopen(argv[4], "wb");

        char type[2];
        int width, height, maximum;

        fscanf(input, "%s", type);
        fscanf(input, "%d %d", &width, &height);
        fscanf(input, "%d", &maximum);
        char ignore;
        fscanf(input, "%c", &ignore);

        // alocare memoria pt matricea imaginii
        Image **image = (Image **)malloc(height * sizeof(Image *) + height * width * sizeof(Image));
        Image *ptr = (Image *)(image + height);

        // distibuira elementele din matrice cu zona corespunzatoare
        for (int i = 0; i < height; i++)
        {
            image[i] = (ptr + width * i);
            fread(image[i], sizeof(Image), width, input);
        }
        fclose(input);

        QuadTree *quad = NULL;
        int nr_nodes = 0;
        CreateTree(&quad, 0, 0, width, factor, image, &nr_nodes);
        QuadtreeNode *nodes = (QuadtreeNode *)malloc(nr_nodes * sizeof(QuadtreeNode));
        int nr_colors = 0, len_cur = 0;
        CreateArray(quad, nodes, &len_cur, &nr_colors);
        printf("%d %d\n", nr_colors, nr_nodes);

        fwrite(&nr_colors, sizeof(int), 1, output);
        fwrite(&nr_nodes, sizeof(int), 1, output);
        fwrite(nodes, sizeof(QuadtreeNode), nr_nodes, output);
        fclose(output);
        free(nodes);
        free(image);
        FreeTree(quad);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        FILE *input = fopen(argv[2], "rb");
        FILE *output = fopen(argv[3], "wb");

        int nr_nodes, nr_colors;
        fread(&nr_colors, sizeof(int), 1, input);
        fread(&nr_nodes, sizeof(int), 1, input);
        QuadtreeNode *nodes = (QuadtreeNode *)malloc(nr_nodes * sizeof(QuadtreeNode));
        fread(nodes, sizeof(QuadtreeNode), nr_nodes, input);
        fclose(input);
        int width, height;
        height = sqrt(nodes[0].area);
        width = height;
        Image **image = (Image **)malloc(height * sizeof(Image *) + height * width * sizeof(Image));
        Image *ptr = (Image *)(image + height);

        for (int i = 0; i < height; i++)
        {
            image[i] = (ptr + width * i);
        }
        QuadTree *quad = NULL;
        ReconstructTree(nodes, &quad, 0);
        ReconstructImage(image, quad, 0, 0);
        fprintf(output, "P6\n");
        fprintf(output, "%d %d\n", height, width);
        fprintf(output, "255\n");
        for (int i = 0; i < height; i++)
        {
            fwrite(image[i], sizeof(Image), width, output);
        }

        fclose(output);
        free(nodes);
        free(image);
        FreeTree(quad);
    }
    else if (strcmp(argv[1], "-m") == 0)
    {
        long long factor = atoi(argv[3]);
        FILE *input = fopen(argv[4], "rb");
        FILE *output = fopen(argv[5], "wb");

        char type[2];
        int width, height, maximum;

        fscanf(input, "%s", type);
        fscanf(input, "%d %d", &width, &height);
        fscanf(input, "%d", &maximum);
        char ignore;
        fscanf(input, "%c", &ignore);

        Image **image = (Image **)malloc(height * sizeof(Image *) + height * width * sizeof(Image));
        Image *ptr = (Image *)(image + height);

        for (int i = 0; i < height; i++)
        {
            image[i] = (ptr + width * i);
            fread(image[i], sizeof(Image), width, input);
        }
        fclose(input);

        QuadTree *quad = NULL;
        int nr_nodes = 0;
        CreateTree(&quad, 0, 0, width, factor, image, &nr_nodes);

        if (strcmp(argv[2], "h") == 0)
        {
            MirrorH(&quad);
        }
        else if (strcmp(argv[2], "v") == 0)
        {
            MirrorV(&quad);
        }

        ReconstructImage(image, quad, 0, 0);

        fprintf(output, "P6\n");
        fprintf(output, "%d %d\n", height, width);
        fprintf(output, "255\n");
        for (int i = 0; i < height; i++)
        {
            fwrite(image[i], sizeof(Image), width, output);
        }

        fclose(output);
        free(image);
        FreeTree(quad);
    }
    return;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VIDEOS 10
#define MAX_TITLE 100
#define MAX_NAME 100

typedef struct {
    int id;
    char titulo[MAX_TITLE];
} Video;

typedef struct {
    int id;
    char nome[MAX_NAME];
    int favoritos[MAX_VIDEOS];
    int qtdFavoritos;
} Usuario;

int comparaUsuario(const void *a, const void *b) {
    return ((Usuario *)a)->id - ((Usuario *)b)->id;
}

int comparaVideo(const void *a, const void *b) {
    return ((Video *)a)->id - ((Video *)b)->id;
}

void cadastrarVideo() {
    Video v;
    FILE *f = fopen("videos.dat", "ab");
    printf("ID do vídeo: ");
    scanf("%d", &v.id);
    getchar();
    printf("Título do vídeo: ");
    fgets(v.titulo, MAX_TITLE, stdin);
    v.titulo[strcspn(v.titulo, "\n")] = 0;
    fwrite(&v, sizeof(Video), 1, f);
    fclose(f);
}

void cadastrarUsuario() {
    Usuario u;
    FILE *f = fopen("usuarios.dat", "ab");
    printf("ID do usuário: ");
    scanf("%d", &u.id);
    getchar();
    printf("Nome do usuário: ");
    fgets(u.nome, MAX_NAME, stdin);
    u.nome[strcspn(u.nome, "\n")] = 0;
    u.qtdFavoritos = 0;
    while (u.qtdFavoritos < MAX_VIDEOS) {
        int vid;
        printf("ID do vídeo favorito (-1 para parar): ");
        scanf("%d", &vid);
        if (vid == -1) break;
        u.favoritos[u.qtdFavoritos++] = vid;
    }
    fwrite(&u, sizeof(Usuario), 1, f);
    fclose(f);
}

void listarVideos() {
    FILE *f = fopen("videos.dat", "rb");
    Video v;
    printf("\n--- Vídeos ---\n");
    while (fread(&v, sizeof(Video), 1, f))
        printf("ID: %d | Título: %s\n", v.id, v.titulo);
    fclose(f);
}

void relatorioUsuariosComVideos() {
    FILE *fu = fopen("usuarios.dat", "rb");
    FILE *fv = fopen("videos.dat", "rb");

    fseek(fv, 0, SEEK_END);
    int qtd = ftell(fv) / sizeof(Video);
    rewind(fv);

    Video *videos = malloc(qtd * sizeof(Video));
    fread(videos, sizeof(Video), qtd, fv);
    fclose(fv);

    Usuario u;
    printf("\n--- Relatório ---\n");
    while (fread(&u, sizeof(Usuario), 1, fu)) {
        printf("\nUsuário %d | %s\nFavoritos:\n", u.id, u.nome);
        for (int i = 0; i < u.qtdFavoritos; i++) {
            int id = u.favoritos[i], found = 0;
            for (int j = 0; j < qtd; j++) {
                if (videos[j].id == id) {
                    printf(" - %s\n", videos[j].titulo);
                    found = 1;
                    break;
                }
            }
            if (!found) printf(" - [ID %d não encontrado]\n", id);
        }
    }
    fclose(fu);
    free(videos);
}

void buscarVideoPorID() {
    FILE *f = fopen("videos.dat", "rb");
    fseek(f, 0, SEEK_END);
    int qtd = ftell(f) / sizeof(Video);
    rewind(f);
    Video *videos = malloc(qtd * sizeof(Video));
    fread(videos, sizeof(Video), qtd, f);
    fclose(f);
    qsort(videos, qtd, sizeof(Video), comparaVideo);

    int id;
    printf("ID do vídeo: ");
    scanf("%d", &id);
    Video chave = {.id = id};
    Video *res = bsearch(&chave, videos, qtd, sizeof(Video), comparaVideo);
    if (res) printf("Título: %s\n", res->titulo);
    else printf("Vídeo não encontrado.\n");
    free(videos);
}

void buscarUsuarioPorID() {
    FILE *f = fopen("usuarios.dat", "rb");
    fseek(f, 0, SEEK_END);
    int qtd = ftell(f) / sizeof(Usuario);
    rewind(f);
    Usuario *usuarios = malloc(qtd * sizeof(Usuario));
    fread(usuarios, sizeof(Usuario), qtd, f);
    fclose(f);
    qsort(usuarios, qtd, sizeof(Usuario), comparaUsuario);

    int id;
    printf("ID do usuário: ");
    scanf("%d", &id);
    Usuario chave = {.id = id};
    Usuario *res = bsearch(&chave, usuarios, qtd, sizeof(Usuario), comparaUsuario);
    if (res) printf("Nome: %s\n", res->nome);
    else printf("Usuário não encontrado.\n");
    free(usuarios);
}

void menu() {
    int opc;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Cadastrar Vídeo\n");
        printf("2. Cadastrar Usuário\n");
        printf("3. Listar Vídeos\n");
        printf("4. Relatório de Usuários com Vídeos\n");
        printf("5. Buscar Vídeo por ID\n");
        printf("6. Buscar Usuário por ID\n");
        printf("0. Sair\n");
        scanf("%d", &opc);
        switch (opc) {
            case 1: cadastrarVideo(); break;
            case 2: cadastrarUsuario(); break;
            case 3: listarVideos(); break;
            case 4: relatorioUsuariosComVideos(); break;
            case 5: buscarVideoPorID(); break;
            case 6: buscarUsuarioPorID(); break;
        }
    } while (opc != 0);
}

int main() {
    menu();
    return 0;
}

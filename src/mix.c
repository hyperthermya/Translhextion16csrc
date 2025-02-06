// src/mix.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h> // Para logs e possíveis mensagens de erro

// Estrutura para representar um arquivo dentro do .MIX
typedef struct {
    char filename[32];
    uint32_t offset;
    uint32_t size;
} MixEntry;

// Função principal de extração
void mix_extract(const char *mix_path, const char *output_dir) {
    FILE *mix_file = fopen(mix_path, "rb");
    if (!mix_file) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Erro ao abrir arquivo .MIX: %s", mix_path);
        return;
    }

    // 1. Ler cabeçalho (exemplo hipotético - ajuste conforme o formato real do .MIX)
    uint32_t num_entries;
    fread(&num_entries, sizeof(uint32_t), 1, mix_file);

    // 2. Ler entradas
    MixEntry *entries = malloc(num_entries * sizeof(MixEntry));
    if (!entries) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Falha na alocação de memória!");
        fclose(mix_file);
        return;
    }

    fread(entries, sizeof(MixEntry), num_entries, mix_file);

    // 3. Extrair cada arquivo
    for (uint32_t i = 0; i < num_entries; i++) {
        char output_path[256];
        snprintf(output_path, sizeof(output_path), "%s/%s", output_dir, entries[i].filename);

        FILE *output_file = fopen(output_path, "wb");
        if (!output_file) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao criar arquivo: %s", output_path);
            continue;
        }

        // Alocar buffer e ler dados
        uint8_t *buffer = malloc(entries[i].size);
        if (!buffer) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha na alocação do buffer!");
            fclose(output_file);
            continue;
        }

        fseek(mix_file, entries[i].offset, SEEK_SET);
        fread(buffer, 1, entries[i].size, mix_file);
        fwrite(buffer, 1, entries[i].size, output_file);

        // Liberar recursos
        free(buffer);
        fclose(output_file);
    }

    // 4. Limpeza final
    free(entries);
    fclose(mix_file);
    SDL_Log("Extração concluída: %s", mix_path);
}
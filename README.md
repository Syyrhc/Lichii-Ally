<!--
# SPDX-FileCopyrightText: Copyright 2025 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

# SPDX-FileCopyrightText: 2018 yuzu Emulator Project
# SPDX-License-Identifier: GPL-2.0-or-later
-->

<p align="center">
  <img src="dist/branding/lichii_icon_with_logo.png" alt="Lichii Ally" width="520">
</p>

<h1 align="center">Lichii Ally</h1>

<p align="center">
  <strong>Edição do emulador Nintendo Switch e Switch 2 otimizada para o ASUS ROG Xbox Ally X</strong><br>
  <em>Handheld-tuned edition of the Lichii Nintendo Switch and Switch 2 emulator for the ASUS ROG Xbox Ally X</em>
</p>

<p align="center">
  <a href="https://github.com/ChrysthianChrisley/Lichii-Ally/releases/latest"><strong>Baixar Última Versão (Releases) / Download Latest Release</strong></a>
</p>

---

## Português (PT-BR)

O **Lichii Ally** é uma distribuição pronta para uso do emulador de código aberto [Lichii](https://github.com/zortos293/Lichii), projetada especificamente para a arquitetura do **ASUS ROG Xbox Ally X** (`AMD Ryzen AI Z2 Extreme`, `Radeon 890M` RDNA 3.5, `24 GB LPDDR5X-8000` e tela de `120 Hz` com `AMD FreeSync Premium VRR`).

Inspirado na proposta de configuração zero da versão **Frizti v3** e atualizado com correções recentes do branch principal `nx2`, o pacote já traz ativados de fábrica todos os parâmetros de desempenho, escalonamento híbrido de núcleos e perfis de sincronização de quadros adequados ao portátil.

### Downloads

Acesse a [página de Releases](https://github.com/ChrysthianChrisley/Lichii-Ally/releases/latest) e escolha o pacote correspondente ao seu processador:

| Dispositivo | Arquivo Recomendado | Arquitetura de Compilação |
|---|---|---|
| **ASUS ROG Xbox Ally X** *(Ryzen AI Z2 Extreme — Zen 5 + Zen 5c, 24 GB RAM)* | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, `mtune=znver5`, ThinLTO + PGO) |
| **ASUS ROG Ally X / ROG Ally Z1 Extreme** *(Ryzen Z1 Extreme — Zen 4)* | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, ThinLTO + PGO) |
| **ASUS ROG Xbox Ally (Base)** *(Ryzen Z2 A — Zen 2)*, **Steam Deck (Windows)** e demais PCs x64 | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |

> **Observação:** Os arquivos terminados em `-debug-symbols-not-needed-to-play.zip` contêm apenas símbolos `.pdb` para depuração de falhas e não são necessários para jogar. Caso uma instalação limpa do Windows solicite bibliotecas de execução C++, execute o instalador `vc_redist.x64.exe` incluído na pasta `Lichii-Ally`.

### Otimizações Técnicas Implementadas

1. **Escalonamento de Threads Híbrido (`Zen 5 + Zen 5c`):**
   - Consulta a topologia física do processador no Windows 11 via `GetLogicalProcessorInformationEx` (`EfficiencyClass`) e desativa o *Power Throttling* (*EcoQoS*) nas threads críticas de emulação.
   - Dedica núcleos de alto desempenho **Zen 5 (até 5.0 GHz)** às threads `GpuBack` e `GpuFront`, isolando os compiladores assíncronos de pipelines Vulkan nos **5 núcleos de eficiência Zen 5c** para evitar quedas de frequência e travamentos durante o jogo.
2. **Gerenciamento de Memória Unificada de 24 GB:**
   - Identifica sistemas integrados com 24 GB de RAM física e dimensiona o orçamento do cache de texturas Vulkan para **7 GiB** (modo Conservador) ou **8 GiB** (modo Agressivo), preservando margem segura para o sistema operacional, memória do console convidado e compilação de shaders.
3. **Controle de Compilação Assíncrona de Shaders em APUs:**
   - Limita os *workers* paralelos de compilação de shaders em GPUs integradas a **6 threads** (em vez de 15), reduzindo picos de consumo de memória e evitando disputa de TDP com a GPU Radeon 890M.
   - Utiliza arquivos de cache dedicados (`vulkan-ally`) com migração automática de caches existentes do Lichii principal e da edição Frizti.
4. **Modo de 40 FPS com Genlock em 120 Hz VRR e Alternância em Tempo Real:**
   - Em *The Legend of Zelda: Ocarina of Time* (com *Dynamic FPS* integrado mantendo a velocidade nativa do jogo), o alvo padrão é **40 FPS** (`25,0 ms`), sincronizado exatamente a cada **3 ciclos de atualização** do painel de 120 Hz com alinhamento de fase de baixa latência.
   - Permite alternar imediatamente entre **40 FPS**, **60 FPS** e **30 FPS** durante a execução, pela barra de status, teclado ou controle.
5. **Integração com a Windows 11 Xbox Full Screen Experience:**
   - Inicia jogos diretamente em **Tela Cheia Sem Bordas** (*Borderless Windowed*), preservando a atuação do FreeSync / VRR.
   - Define combinações de botões no controle que não utilizam o botão Guia/Xbox, evitando conflitos com a Xbox Game Bar.
6. **Compilação `clang-cl` 22 + ThinLTO + PGO + Correções Recentes do `nx2`:**
   - Binários gerados com LLVM 22.1.8, *ThinLTO* e *Profile-Guided Optimization* (`lichii-windows.profdata`), incluindo substituição automática de **DLSS por AMD FSR 3.1**, correção de páginas de memória no Windows (`#485`), expansão de RAM de macros Turing (`#439`) e sincronização de encerramento do *GPU Split* (`#438`).

### Configurações Recomendadas no Sistema (ROG Xbox Ally X)

| Componente | Configuração Recomendada | Motivo Técnico |
|---|---|---|
| **Armoury Crate SE — VRAM** | **8 GB** | Equilibra 8 GB dedicados à Radeon 890M com 16 GB livres para Windows, JIT e cache de pipelines. |
| **Tela do Windows / Painel** | **1080p @ 120 Hz + FreeSync (VRR) Ativado** | Permite o travamento exato de 40 FPS (`120 Hz / 3`) e absorve oscilações pontuais de quadros. |
| **AMD Software: Adrenalin** | **Desativar Anti-Lag, Radeon Boost e AFMF** | Evita interferência no pacer interno de baixa latência (*Genlock*) do emulador. |
| **Perfil de Energia (TDP)** | **17W–20W** (40 FPS) / **25W–30W** (60 FPS) / **13W–15W** (30 FPS) | Ajusta o equilíbrio entre autonomia da bateria de 80 Wh e taxa de quadros alvo. |

### Atalhos de Controle e Teclado

| Ação | Atalho no Controle | Atalho no Teclado |
|---|---|---|
| Alternar taxa de quadros alvo (**40 / 60 / 30 FPS**) | `View + R3` *(Clique do Analógico Direito)* | `F7` *(ou botão na barra inferior)* |
| Alternar Tela Cheia Sem Bordas | `View + L3` *(Clique do Analógico Esquerdo)* | `F11` |
| Exibir / ocultar Overlay de Desempenho | `Menu + R3` *(Clique do Analógico Direito)* | `Ctrl + V` |
| Pausar / Retomar Emulação | `Home + Plus` | `F4` |
| Encerrar Emulação | `L + Plus + Minus` | `F5` |

### Dados Salvos e Aviso Legal

O Lichii Ally mantém o diretório padrão de dados `%APPDATA%\eden`, preservando integralmente saves, configurações, firmwares e listas de jogos de instalações anteriores do Lichii, Eglantine ou Eden. Faça backup periódico dos seus dados salvos.

Utilize exclusivamente jogos, chaves criptográficas (*keys*) e arquivos de sistema (*firmware*) obtidos legalmente a partir do seu próprio console. Nenhum jogo, chave ou firmware é distribuído neste repositório.

---

## English

**Lichii Ally** is a pre-configured distribution of the open-source [Lichii](https://github.com/zortos293/Lichii) Nintendo Switch and Switch 2 emulator, engineered specifically for the **ASUS ROG Xbox Ally X** (`AMD Ryzen AI Z2 Extreme`, `Radeon 890M` RDNA 3.5, `24 GB LPDDR5X-8000`, and `120 Hz` `AMD FreeSync Premium VRR` display).

### Downloads

Visit the [Releases page](https://github.com/ChrysthianChrisley/Lichii-Ally/releases/latest) and select the package matching your hardware:

- **`Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip`**: Compiled with `clang-cl` 22.1.8, ThinLTO, PGO, and `/clang:-march=x86-64-v4 /clang:-mtune=znver5`. Recommended for **ASUS ROG Xbox Ally X**, **ROG Ally X**, and **ROG Ally Z1 Extreme**.
- **`Lichii-Ally-vX-Universal-AVX2.zip`**: Compiled with `clang-cl` 22.1.8, ThinLTO, PGO, and `/clang:-march=x86-64-v3`. Recommended for the base **ASUS ROG Xbox Ally** (Ryzen Z2 A), **Steam Deck (Windows)**, and standard x86_64 PCs without AVX-512.

### Summary of Engineering Changes

- **Windows 11 Hybrid Core Scheduling (`Zen 5 + Zen 5c`):** Queries hardware `EfficiencyClass` masks to disable Windows EcoQoS on critical emulation threads, pins `GpuBack` and `GpuFront` to full **Zen 5** performance cores, and isolates background Vulkan shader compilation onto **Zen 5c** efficiency cores.
- **24 GB Unified Memory Allocation:** Detects 24 GB integrated APU hosts and assigns a **7–8 GiB** Vulkan texture cache budget while preserving headroom for Windows and guest memory. Set **8 GB VRAM** in Armoury Crate SE for optimal balance.
- **Handheld-Tuned Async Shader Compilation:** Caps parallel Vulkan pipeline workers to **6** on integrated GPUs to prevent CPU power starvation and system RAM spikes, while migrating existing `vulkan-frizti` and `vulkan` pipeline caches automatically.
- **120 Hz VRR 40 FPS Target & Live Switching:** Sets the default *Ocarina of Time* frame rate target to **40 FPS** (locked to every 3rd refresh on 120 Hz VRR displays with low-latency phase alignment) and supports live switching across **40 / 60 / 30 FPS** via **`View + R3`**, **`F7`**, or the status bar selector.
- **Xbox Full Screen Experience Integration:** Boots games directly into Borderless Windowed Fullscreen on Windows and binds controller shortcuts (`View + R3`, `View + L3`, `Menu + R3`) without relying on the Xbox Guide button.

### Building from Source

See [docs/Build.md](docs/Build.md) and [docs/Coding.md](docs/Coding.md) for general build requirements. To configure an optimized Windows build with `clang-cl` targeting `x86-64-v4` with Zen 5 scheduling:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DYUZU_BUILD_PRESET=v4
cmake --build build --target yuzu yuzu-cmd --parallel
```

---

## Credits and License

Lichii Ally builds upon the work of:
- **[Lichii](https://github.com/zortos293/Lichii)** by Zortos and contributors
- **[Frizti's Lichii Builds](https://github.com/Frizti/Lichii)** (pre-configured Ocarina of Time workflow, Dynamic FPS integration with **Dark**, and shader workarounds with **Evil**)
- **[Eglantine](https://github.com/Djihads80/Eglantine)** (Camille LaVey), **Eden**, **yuzu**, and **Sudachi**

Application branding artwork in [dist/branding](dist/branding) was created by **YOB (yobslab)**. Distributed under the **GPL-3.0-or-later** license; see [LICENSE.txt](LICENSE.txt) for full terms. This project is independent and is not affiliated with Nintendo, ASUS, or Microsoft.

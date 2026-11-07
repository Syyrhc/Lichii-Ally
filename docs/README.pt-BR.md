<!--
# SPDX-FileCopyrightText: Copyright 2025 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later
-->

<p align="center">
  <img src="../dist/branding/lichii_icon_with_logo.png" alt="Lichii Ally" width="520">
</p>

<h1 align="center">Lichii Ally</h1>

<p align="center">
  <strong>Edição do emulador Nintendo Switch e Switch 2 otimizada para o ASUS ROG Xbox Ally X</strong>
</p>

<p align="center">
  <a href="https://github.com/ChrysthianChrisley/Lichii-Ally/releases/latest"><strong>Baixar Última Versão (Releases)</strong></a>
  &nbsp;&middot;&nbsp;
  <a href="../README.md">English</a>
</p>

---

O **Lichii Ally** é uma distribuição pronta para uso do emulador de código aberto [Lichii](https://github.com/zortos293/Lichii), projetada especificamente para a arquitetura do **ASUS ROG Xbox Ally X** (`AMD Ryzen AI Z2 Extreme`, `Radeon 890M` RDNA 3.5, `24 GB LPDDR5X-8000` e tela de `120 Hz` com `AMD FreeSync Premium VRR`).

## Downloads

Acesse a [página de Releases](https://github.com/ChrysthianChrisley/Lichii-Ally/releases/latest), extraia a pasta `Lichii-Ally` e execute `lichii.exe`:

| Dispositivo | Arquivo Recomendado | Arquitetura de Compilação |
|---|---|---|
| **ASUS ROG Xbox Ally X** *(Ryzen AI Z2 Extreme — Zen 5 + Zen 5c, 24 GB RAM)* | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, `mtune=znver5`, ThinLTO + PGO) |
| **ASUS ROG Ally X / ROG Ally Z1 Extreme** *(Ryzen Z1 Extreme — Zen 4)* | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, ThinLTO + PGO) |
| **ASUS ROG Xbox Ally (Base)** *(Ryzen Z2 A — Zen 2)*, **Steam Deck (Windows)** e demais PCs x64 | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |

> **Observação:** Os arquivos terminados em `-debug-symbols-not-needed-to-play.zip` contêm apenas símbolos `.pdb` para depuração de falhas e não são necessários para jogar. Caso o Windows solicite bibliotecas C++, execute o `vc_redist.x64.exe` incluído na pasta.

## Otimizações Técnicas

1. **Escalonamento Híbrido `Zen 5 + Zen 5c`:** Desativa o *EcoQoS* do Windows nas threads críticas, reserva núcleos **Zen 5** para `GpuBack` e `GpuFront` e isola a compilação assíncrona de shaders nos **5 núcleos Zen 5c**.
2. **Orçamento de Memória para 24 GB:** Eleva o orçamento do cache de texturas Vulkan em APUs de 24 GB para **7–8 GiB**, mantendo margem segura para o Windows e shaders.
3. **Controle de Shaders em iGPUs:** Limita os *workers* assíncronos de pipeline a **6 threads** em GPUs integradas (evitando estrangulamento térmico de TDP) e migra caches existentes automaticamente para `vulkan-ally`.
4. **Modo 40 FPS em 120 Hz VRR + Alternância em Tempo Real:** Alvo padrão de **40 FPS** em *Ocarina of Time* (travado a cada 3 atualizações do painel de 120 Hz com baixa latência), alternável ao vivo entre **40 / 60 / 30 FPS**.
5. **Integração com a Xbox Full Screen Experience:** Inicia jogos em Tela Cheia Sem Bordas por padrão e utiliza atalhos de controle sem conflito com a Xbox Game Bar.

## Configurações Recomendadas (ROG Xbox Ally X)

| Componente | Configuração Recomendada | Motivo Técnico |
|---|---|---|
| **Armoury Crate SE — VRAM** | **8 GB** | Equilibra 8 GB dedicados à Radeon 890M com 16 GB livres para Windows, JIT e shaders. |
| **Tela do Windows / Painel** | **1080p @ 120 Hz + FreeSync (VRR) Ativado** | Permite o travamento exato de 40 FPS (`120 Hz / 3`) sem *judder*. |
| **AMD Software: Adrenalin** | **Desativar Anti-Lag, Radeon Boost e AFMF** | Evita interferência no pacer interno de baixa latência do emulador. |
| **Perfil de Energia (TDP)** | **17W–20W** (40 FPS) / **25W–30W** (60 FPS) / **13W–15W** (30 FPS) | Equilibra autonomia da bateria de 80 Wh e taxa de quadros alvo. |

## Atalhos de Controle e Teclado

| Ação | Controle | Teclado |
|---|---|---|
| Alternar taxa de quadros alvo (**40 / 60 / 30 FPS**) | `View + R3` | `F7` *(ou botão na barra inferior)* |
| Alternar Tela Cheia Sem Bordas | `View + L3` | `F11` |
| Exibir / ocultar Overlay de Desempenho | `Menu + R3` | `Ctrl + V` |
| Pausar / Retomar Emulação | `Home + Plus` | `F4` |
| Encerrar Emulação | `L + Plus + Minus` | `F5` |

## Aviso Legal e Licença

Utilize exclusivamente jogos, chaves (*keys*) e *firmware* extraídos legalmente do seu próprio console. Distribuído sob a licença **GPL-3.0-or-later** ([LICENSE.txt](../LICENSE.txt)).

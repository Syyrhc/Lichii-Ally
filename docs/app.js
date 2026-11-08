"use strict";

const messages = {
  main: ["Main Lichii", "Lichii principal"],
  mainLabel: ["Main release", "Release principal"],
  allyLabel: ["Windows handhelds", "Portáteis Windows"],
  prereleaseLabel: ["Pre-release", "Pré-release"],
  mainReason: [
    "The main release is a starting point for general use, with packages for Windows x64, Linux and macOS on Apple Silicon.",
    "A release principal é o ponto de partida para uso geral, com pacotes para Windows x64, Linux e macOS em Apple Silicon.",
  ],
  unsupportedTitle: ["No native package found", "Sem pacote nativo identificado"],
  unsupportedLabel: ["System outside this selection", "Sistema fora da seleção"],
  unsupportedReason: [
    "The releases in this guide provide packages for Windows x64, Linux x64 or ARM64, and macOS 15 or later on Apple Silicon.",
    "As releases comparadas oferecem pacotes para Windows x64, Linux x64 ou ARM64 e macOS 15+ em Apple Silicon.",
  ],
  unsupportedNote: [
    "Check your system architecture or browse the release catalog.",
    "Confira a arquitetura do seu sistema ou consulte o catálogo de releases.",
  ],
  allyReason: [
    "Lichii Ally includes controller and shared-memory tuning for Windows handhelds.",
    "O Lichii Ally tem ajustes de controle e memória compartilhada para portáteis Windows.",
  ],
  steamDeckLinux: [
    "For a Steam Deck running SteamOS, start with the Linux x64 AppImage from Main Lichii.",
    "Para o Steam Deck com SteamOS, comece pelo AppImage Linux x64 do Lichii principal.",
  ],
  steamDeckWindows: [
    "For a Steam Deck running Windows, use the Universal AVX2 edition of Lichii Ally. It matches the Deck’s AMD Zen 2 CPU.",
    "Para o Steam Deck com Windows, use a edição Universal AVX2 do Lichii Ally. Ela corresponde à CPU AMD Zen 2 do aparelho.",
  ],
  avx512: ["The AVX-512 edition matches the CPU indicated.", "A edição AVX-512 corresponde à CPU indicada."],
  avx2: ["The Universal AVX2 edition matches the CPU indicated.", "A edição Universal AVX2 corresponde à CPU indicada."],
  confirmCpu: [
    "Check your CPU model. The Universal edition requires AVX2 and the AVX-512 edition requires AVX-512.",
    "Confirme o modelo da CPU. A edição Universal exige AVX2 e a edição AVX-512 exige suporte a AVX-512.",
  ],
  ootReason: [
    "This pre-release targets Ocarina of Time on Radeon GPUs. The published tests used an RX 9070 XT.",
    "Esta pré-release é voltada a Ocarina of Time em Radeon. Os testes publicados usaram uma RX 9070 XT.",
  ],
  amdReason: [
    "This pre-release includes Radeon graphics fixes and an on-disk shader cache for drivers with VK_KHR_pipeline_binary support.",
    "Esta pré-release reúne correções para GPUs Radeon e cache de shaders em disco com drivers que oferecem VK_KHR_pipeline_binary.",
  ],
  fritchiiReason: [
    "The Fritchii pre-release focuses on Ocarina of Time on Windows. The author documented testing with an RTX 4080.",
    "A pré-release Fritchii é voltada a Ocarina of Time no Windows. O autor documenta testes com uma RTX 4080.",
  ],
  ambiguousAmd: [
    "I interpreted “AMD” as a Radeon GPU. If AMD is your CPU brand, enter your graphics card to refine the result.",
    "Interpretei “AMD” como GPU Radeon. Se AMD for a marca do seu processador, informe a placa de vídeo para refinar a escolha.",
  ],
  multipleGpus: [
    "The entry lists GPUs from different brands. Enter the GPU the emulator will use to evaluate a specialized build.",
    "Foram identificadas GPUs de marcas diferentes. Informe apenas a GPU que o emulador vai usar para avaliar um fork específico.",
  ],
  multipleSystems: [
    "Choose one operating system under “Refine recommendation” to find its package.",
    "Escolha um sistema em “Refinar recomendação” para identificar o pacote.",
  ],
  inferredWindows: [
    "Use this package if the handheld is running Windows x64. If you use Linux, select it in the form.",
    "Use este pacote se o portátil estiver rodando Windows x64. Se usa Linux, selecione o sistema no formulário.",
  ],
  inferredSteamOs: [
    "If you installed Windows, select Windows x64 to see the Ally Universal AVX2 download.",
    "Se você instalou o Windows, selecione Windows x64 para ver o download do Ally Universal AVX2.",
  ],
  legacyCpu: [
    "Lichii Ally requires at least AVX2. Check the CPU requirements for the linked release before downloading.",
    "O Lichii Ally exige pelo menos AVX2. Confira os requisitos de CPU da release antes de baixar.",
  ],
  handheldSystem: [
    "Select your handheld’s operating system. Lichii Ally is designed for Windows x64 handhelds with a supported CPU.",
    "Selecione o sistema do portátil. O Lichii Ally é a opção específica para portáteis Windows x64 com CPU compatível.",
  ],
  fritchiiSystem: [
    "If you use Windows x64, select it to see the Fritchii recommendation.",
    "Se usa Windows x64, selecione o sistema para avaliar a Fritchii.",
  ],
  selectSystem: [
    "The shortcut below is for Windows x64. Enter or select your operating system to see a different package.",
    "O atalho abaixo é para Windows x64. Informe seu sistema para ver outro pacote.",
  ],
  macRequirements: [
    "The package requires macOS 15 or later on Apple Silicon.",
    "O pacote requer macOS 15 ou posterior em Apple Silicon.",
  ],
  linuxRequirements: [
    "The AppImage requires glibc 2.39 or later. Check the release for the .tar.gz and its dependencies.",
    "O AppImage exige glibc 2.39 ou posterior. Consulte também o .tar.gz e as dependências na release.",
  ],
  allyRam24: [
    "The release describes cache tuning for handhelds with 24 GB of RAM or more.",
    "Com 24 GB ou mais, a release descreve ajustes de cache para portáteis nessa faixa.",
  ],
  allyRam16: [
    "The release includes a profile for 16 GB handhelds that share RAM with the GPU.",
    "A release inclui um perfil para portáteis de 16 GB que dividem a RAM com a GPU.",
  ],
  lowRam: [
    "With under 16 GB, check how much RAM remains after any allocation to an integrated GPU. The release notes give no per-build RAM minimum.",
    "Com menos de 16 GB, considere a RAM disponível após a reserva da GPU integrada. As releases não publicam um mínimo de RAM por build.",
  ],
  download: ["Download", "Baixar para"],
  viewRelease: ["View release", "Ver release"],
  compare: ["Compare builds", "Comparar builds"],
  evidence: ["Read build notes", "Ver documentação ou notas da build"],
};

let language = "en";

function translate(key) {
  return messages[key][language === "pt-BR" ? 1 : 0];
}

const desktopPackages = {
  windows: ["lichii-windows-x86_64-pgo.zip", "Windows x64 · ZIP PGO", "windows"],
  linux: ["lichii-linux-x86_64-pgo.AppImage", "Linux x64 · AppImage PGO"],
  linuxArm: ["lichii-linux-aarch64-pgo.AppImage", "Linux ARM64 · AppImage PGO"],
  mac: ["lichii-macos-arm64.zip", "macOS Apple Silicon · ZIP"],
};

const windowsStandard = ["lichii-windows-x86_64.zip", "Windows x64", "windows"];

const allyPackages = {
  avx512: ["Lichii-Ally-v3-Xbox-Ally-X-AVX512.zip", "Windows x64 · AVX-512", "windows"],
  avx2: ["Lichii-Ally-v3-Universal-AVX2.zip", "Windows x64 · AVX2", "windows"],
};

const handhelds = [
  [/\b(?:rog )?(?:xbox )?ally x\b/, "windows", "avx512", "amd"],
  [/\b(?:rog )?xbox ally\b/, "windows", "avx2", "amd"],
  [/\b(?:rog )?ally\b/, "windows", "avx512", "amd"],
  [/\bsteam deck\b/, "linux", "avx2", "amd"],
  [/\blegion go s\b/, "", "", "amd"],
  [/\blegion go(?: 2)?\b/, "windows", "avx512", "amd"],
  [/\b(?:msi )?claw a8\b/, "windows", "avx512", "amd"],
  [/\b(?:msi )?claw (?:a1m|[78] ai\+?)/, "windows", "avx2", "intel"],
  [/\bmsi claw\b/, "windows", "", ""],
  [/\b(?:gpd win|ayaneo|onexplayer|handheld|console portatil)\b/, "", "", ""],
];

function readHardware(text) {
  const hardware = text.normalize("NFD").replace(/[\u0300-\u036f]/g, "")
    .toLowerCase().replace(/[-‐–—]/g, " ").trim();
  const handheld = handhelds.find(([pattern]) => pattern.test(hardware));
  const consoleCpu = /\bz(?:1|2)(?: extreme| go| a)?\b/.test(hardware);
  const amdGpu = /\b(?:radeon|rx\s*\d{3,4}(?:xtx?)?)\b/.test(hardware);
  const nvidiaGpu = /\b(?:nvidia|geforce|rtx|gtx)\s*\d*\b/.test(hardware);
  const intelGpu = /\b(?:intel arc|arc\s*[ab]\d{3}|iris(?: xe)?|intel uhd)\b/.test(hardware);
  const ambiguousAmd = /\bamd\b/.test(hardware)
    && !/\b(?:ryzen|zen|cpu|processador|athlon|epyc|threadripper)\b/.test(hardware)
    && !amdGpu && !nvidiaGpu && !intelGpu && !handheld && !consoleCpu;
  const multipleGpus = [amdGpu, nvidiaGpu, intelGpu].filter(Boolean).length > 1;

  let gpu = handheld?.[3] || "";
  if (intelGpu) gpu = "intel";
  if (amdGpu || ambiguousAmd) gpu = "amd";
  if (nvidiaGpu) gpu = "nvidia";
  if (multipleGpus) gpu = "";

  let cpu = handheld?.[2] || "";
  if (/\b(?:z2 a|z2 go|core ultra|zen [23]\+?|avx2)\b/.test(hardware)) cpu = "avx2";
  if (/\b(?:z[12] extreme|z1|zen [45]|hx 370|[78]840u|avx\s*512)\b/.test(hardware)) cpu = "avx512";
  if (/\b(?:sem avx2|no avx2)\b/.test(hardware)) cpu = "legacy";

  const windows = /\b(?:windows|win(?:10|11|64))\b/.test(hardware);
  const linux = /\b(?:linux|steamos|bazzite|ubuntu|fedora|debian)\b/.test(hardware);
  const mac = /\b(?:macos|macbook|mac|apple (?:silicon|m[1-9]))\b/.test(hardware);
  const arm = /\b(?:arm64|aarch64|arm|snapdragon)\b/.test(hardware);
  const unsupported = /\b(?:android|ios|iphone|ipad|chromebook|chrome os|windows 32|win32)\b/.test(hardware)
    || (windows && arm) || (mac && /\bintel\b/.test(hardware));
  const multipleSystems = [windows, linux, mac].filter(Boolean).length > 1;
  let system = "";
  if (windows) system = "windows";
  if (linux) system = arm ? "linuxArm" : "linux";
  if (mac) system = "mac";
  if (unsupported) system = "unsupported";
  if (multipleSystems) system = "";

  const ramText = hardware.replace(/\bvram\s*[:=]?\s*\d{1,3}\s*gb\b|\b\d{1,3}\s*gb\s*(?:de\s+)?vram\b/g, "");
  const ram = ramText.match(/\b(\d{1,3})\s*gb\b/);
  return {
    gpu, cpu, system, ambiguousAmd, multipleGpus, multipleSystems,
    steamDeck: /\bsteam deck\b/.test(hardware),
    handheld: Boolean(handheld || consoleCpu),
    deviceSystem: handheld?.[1] || (consoleCpu ? "windows" : ""),
    macChip: /\b(?:apple silicon|m[1-9])\b/.test(hardware),
    oot: /\b(?:ocarina(?: of time)?|oot)\b/.test(hardware),
    ram: ram ? Number(ram[1]) : 0,
  };
}

function recommend(profile, options) {
  const inferredSystem = !options.system && !profile.system && !profile.multipleSystems;
  const system = options.system || profile.system || (inferredSystem ? profile.deviceSystem : "");
  const cpu = profile.steamDeck ? "avx2" : options.cpu || profile.cpu;
  const ram = Number(options.ram) || profile.ram;
  const oot = options.game === "oot" || profile.oot;
  const notes = [];
  let key = "main";
  let reason = "mainReason";
  let pkg = desktopPackages[system];
  if (!system && !profile.handheld && !profile.multipleSystems) pkg = windowsStandard;

  if (system === "unsupported") {
    return {
      title: translate("unsupportedTitle"),
      label: translate("unsupportedLabel"),
      reason: translate("unsupportedReason"),
      notes: [translate("unsupportedNote")],
    };
  }

  if (profile.steamDeck && system === "linux") {
    reason = "steamDeckLinux";
  } else if (profile.handheld && system === "windows" && cpu !== "legacy") {
    key = "ally";
    reason = profile.steamDeck ? "steamDeckWindows" : "allyReason";
    pkg = allyPackages[cpu];
    if (!profile.steamDeck) notes.push(pkg ? cpu : "confirmCpu");
  } else if (profile.gpu === "amd" && system !== "mac" && (!profile.handheld || oot || system === "windows")) {
    key = oot ? "oot" : "amd";
    reason = oot ? "ootReason" : "amdReason";
  } else if (oot && system === "windows" && profile.gpu === "nvidia") {
    key = "fritchii";
    reason = "fritchiiReason";
    pkg = ["Fritchii-v4.1-Windows-x64.zip", "Windows x64 · ZIP", "windows"];
  }

  if (profile.ambiguousAmd) notes.push("ambiguousAmd");
  if (profile.multipleGpus) notes.push("multipleGpus");
  if (profile.multipleSystems && !options.system) notes.push("multipleSystems");
  if (inferredSystem && system === "windows") notes.push("inferredWindows");
  if (inferredSystem && system === "linux") notes.push("inferredSteamOs");
  if (profile.handheld && system === "windows" && cpu === "legacy") notes.push("legacyCpu");
  if (profile.handheld && !system && !profile.multipleSystems) notes.push("handheldSystem");
  if (!profile.handheld && !system && !profile.multipleSystems) {
    notes.push(oot && profile.gpu === "nvidia" ? "fritchiiSystem" : "selectSystem");
  }
  if (system === "mac") {
    notes.push("macRequirements");
    if (!options.system && !profile.macChip) pkg = undefined;
  }
  if (system === "linux" || system === "linuxArm") notes.push("linuxRequirements");
  if (key === "ally" && ram >= 24) notes.push("allyRam24");
  if (key === "ally" && ram >= 16 && ram < 24) notes.push("allyRam16");
  if (ram > 0 && ram < 16) notes.push("lowRam");

  const build = builds[key];
  return {
    key,
    title: build.title || translate("main"),
    label: translate(build.prereleases ? "prereleaseLabel" : key + "Label"),
    reason: translate(reason),
    notes: notes.map(translate),
    pkg,
  };
}

const form = document.querySelector("#selector");
const hardwareInput = form.elements.namedItem("hardware");
const result = document.querySelector("#result");
const startContent = result.innerHTML;
const presets = [...form.querySelectorAll("[data-profile]")];
const languageButtons = [...document.querySelectorAll("[data-language]")];
const staticEnglish = new Map();
let currentOptions = null;
let releaseView = 0;

function translateStatic(root) {
  const bindings = [
    ["data-i18n", ""],
    ["data-i18n-placeholder", "placeholder"],
    ["data-i18n-aria-label", "aria-label"],
    ["data-i18n-content", "content"],
    ["data-i18n-title", "title"],
  ];
  for (const [binding, attribute] of bindings) {
    for (const node of root.querySelectorAll("[" + binding + "]")) {
      const key = node.getAttribute(binding);
      if (!staticEnglish.has(key)) {
        staticEnglish.set(key, attribute ? node.getAttribute(attribute) : node.textContent);
      }
      const english = staticEnglish.get(key);
      const translation = language === "pt-BR" ? staticPortuguese[key] ?? english : english;
      if (attribute) node.setAttribute(attribute, translation);
      else node.textContent = translation;
    }
  }
}

function element(tag, className, content) {
  const node = document.createElement(tag);
  node.className = className;
  node.textContent = content;
  return node;
}

function action(label, href, style, platform, version) {
  const link = element("a", "button " + style, "");
  if (platform === "windows") {
    const icon = element("img", "platform-icon", "");
    icon.src = "./windows.svg";
    icon.alt = "";
    icon.width = 16;
    icon.height = 16;
    link.append(icon);
  }
  const text = element("span", "", label);
  if (version) {
    const versionText = element("small", "download-version", version);
    versionText.dataset.downloadVersion = "";
    text.className = "download-copy";
    text.append(versionText);
    link.dataset.downloadLink = "";
  }
  link.append(text);
  const arrow = element("span", "", "↗");
  arrow.setAttribute("aria-hidden", "true");
  link.href = href;
  if (href.startsWith("https://")) {
    link.target = "_blank";
    link.rel = "noopener";
  }
  link.append(arrow);
  return link;
}

function render(recommendation) {
  const { key, title, label, reason, notes, pkg } = recommendation;
  const build = builds[key];
  const heading = element("h2", "", title);
  heading.id = "result-title";
  const badge = element("span", "result-badge", label);
  const children = [badge, heading, element("p", "result-description", reason)];
  const actions = element("div", "result-actions", "");
  if (build) {
    const links = releaseLinks(build, build.tag, pkg?.[0]);
    if (pkg) {
      const [, packageLabel, platform] = pkg;
      actions.append(action(translate("download") + " " + packageLabel, links.downloadUrl, "button-lime", platform, links.version));
    } else {
      badge.textContent = links.version + " · " + label;
    }
    const releaseLink = action(translate("viewRelease"), links.releaseUrl, pkg ? "button-outline" : "button-lime");
    releaseLink.dataset.releaseLink = "";
    actions.append(releaseLink);
  } else {
    actions.append(action(translate("compare"), "#builds", "button-lime"));
  }
  children.push(actions);
  if (notes.length) {
    const box = element("div", "result-notes", "");
    box.append(...notes.map((note) => element("p", "", note)));
    children.push(box);
  }
  if (build?.source) {
    const evidence = element("a", "result-source", translate("evidence"));
    evidence.href = build.source;
    children.push(evidence);
  }
  result.replaceChildren(...children);
}

async function refreshResult() {
  const view = ++releaseView;
  const recommendation = currentOptions
    ? recommend(readHardware(currentOptions.hardware), currentOptions)
    : { key: "main", pkg: windowsStandard };
  if (currentOptions) render(recommendation);
  else {
    result.innerHTML = startContent;
    translateStatic(result);
  }
  const { key, pkg, label } = recommendation;
  if (!key) return;

  const update = await findUpdatedRelease(key, pkg);
  if (!update || view !== releaseView) return;
  result.querySelector("[data-release-link]").href = update.releaseUrl;
  if (pkg) {
    result.querySelector("[data-download-link]").href = update.downloadUrl;
    result.querySelector("[data-download-version]").textContent = update.version;
  } else {
    result.querySelector(".result-badge").textContent = update.version + " · " + label;
  }
}

function updatePresets() {
  for (const preset of presets) {
    preset.setAttribute("aria-pressed", String(preset.dataset.profile === hardwareInput.value));
  }
}

form.addEventListener("submit", (event) => {
  event.preventDefault();
  const options = Object.fromEntries(new FormData(form));
  currentOptions = Object.values(options).some((value) => value.trim()) ? options : null;
  refreshResult();
  if (currentOptions && window.matchMedia("(max-width: 760px)").matches) {
    result.closest("section").scrollIntoView({ block: "nearest" });
  }
  if (!currentOptions) hardwareInput.focus();
  updatePresets();
});

for (const preset of presets) {
  preset.addEventListener("click", () => {
    hardwareInput.value = preset.dataset.profile;
    form.requestSubmit();
  });
}

hardwareInput.addEventListener("input", updatePresets);
form.addEventListener("reset", () => {
  currentOptions = null;
  refreshResult();
  for (const preset of presets) preset.setAttribute("aria-pressed", "false");
});

translateStatic(document);
refreshResult();
for (const button of languageButtons) {
  button.addEventListener("click", () => {
    language = button.dataset.language;
    document.documentElement.lang = language;
    for (const option of languageButtons) {
      option.setAttribute("aria-pressed", String(option === button));
    }
    translateStatic(document);
    refreshResult();
  });
}

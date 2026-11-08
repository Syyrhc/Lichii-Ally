"use strict";

const builds = {
  main: {
    repository: "zortos293/Lichii",
    tag: "build-117",
    tags: /^build-\d+$/,
  },
  amd: {
    title: "AMD Superior",
    repository: "CheezerWheezer/Lichii",
    tag: "build-6",
    tags: /^build-\d+$/,
    branches: /^amd-superior$/,
    prereleases: true,
    source: "https://github.com/CheezerWheezer/Lichii/blob/amd-superior/AMD-SUPERIOR.md",
  },
  ally: {
    title: "Lichii Ally",
    repository: "Syyrhc/Lichii-Ally",
    tag: "ally-v3",
    tags: /^ally-v\d+(?:\.\d+)*$/,
    source: "https://github.com/Syyrhc/Lichii-Ally#downloads-and-handheld-compatibility",
  },
  oot: {
    title: "Lichii OoT AMD",
    repository: "conongnang/Lichii",
    tag: "build-5",
    tags: /^build-\d+$/,
    branches: /^(?:oot-amd-perf|perf\/oot-amd-[\w-]+)$/,
    prereleases: true,
    source: "https://github.com/conongnang/Lichii/releases/tag/build-3",
  },
  fritchii: {
    title: "Fritchii",
    repository: "Frizti/Fritchii",
    tag: "fritchii-v4.1",
    tags: /^fritchii-v\d+(?:\.\d+)*$/,
    prereleases: true,
    source: "https://github.com/Frizti/Fritchii/releases/tag/fritchii-v4",
  },
};

const releaseRequests = new Map();

function releaseVersion(tag) {
  return tag.replace(/^(?:ally-|fritchii-)/, "").replace(/^build-/, "Build ");
}

function releaseLinks(build, tag, filename) {
  const base = "https://github.com/" + build.repository + "/releases";
  return {
    version: releaseVersion(tag),
    releaseUrl: base + "/tag/" + tag,
    downloadUrl: filename ? base + "/download/" + tag + "/" + filename : "",
  };
}

function compareTags(left, right) {
  const numbers = (tag) => tag.match(/\d+(?:\.\d+)*/)[0].split(".").map(Number);
  const a = numbers(left);
  const b = numbers(right);
  for (let index = 0; index < Math.max(a.length, b.length); index++) {
    const difference = (a[index] || 0) - (b[index] || 0);
    if (difference) return difference;
  }
  return 0;
}

async function fetchReleases(build) {
  const endpoint = "https://api.github.com/repos/" + build.repository + "/releases"
    + (build.prereleases ? "?per_page=10" : "/latest");
  const response = await fetch(endpoint, {
    credentials: "omit",
    signal: AbortSignal.timeout(5000),
  });
  if (!response.ok) return [];
  const data = await response.json();
  return Array.isArray(data) ? data : [data];
}

async function findUpdatedRelease(key, pkg) {
  const build = builds[key];
  if (!releaseRequests.has(key)) {
    releaseRequests.set(key, fetchReleases(build).catch(() => []));
  }
  const releases = await releaseRequests.get(key);
  const candidates = releases.filter((release) => release?.published_at
    && !release.draft
    && (!release.prerelease || build.prereleases)
    && build.tags.test(release.tag_name)
    && (!build.branches || build.branches.test(release.target_commitish))
    && compareTags(release.tag_name, build.tag) > 0);
  candidates.sort((a, b) => compareTags(b.tag_name, a.tag_name));

  for (const release of candidates) {
    const tag = release.tag_name;
    const filename = pkg?.[0].replace(releaseVersion(build.tag), releaseVersion(tag));
    const links = releaseLinks(build, tag, filename);
    if (!filename || (Array.isArray(release.assets) && release.assets.some((asset) =>
      asset?.state === "uploaded" && asset.name === filename && asset.browser_download_url === links.downloadUrl))) {
      return links;
    }
  }
  return null;
}

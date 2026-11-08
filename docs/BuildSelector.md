# Seletor de builds Lichii

A página está em `site/` e usa HTML, CSS e JavaScript sem dependências. Para testar localmente, abra `site/index.html` no navegador.

## Publicação no GitHub Pages

1. Envie os arquivos da página e o workflow `.github/workflows/build-selector-pages.yml` à branch padrão do repositório que vai hospedar o site.
2. Em **Settings → Pages → Build and deployment**, escolha **GitHub Actions** como fonte.
3. Em **Actions → Publicar seletor no GitHub Pages**, clique em **Run workflow**. A execução mostra o endereço do site depois da publicação.

O workflow publica a pasta `site/` como raiz do site. O seletor usa caminhos relativos e funciona em endereços do tipo `https://<usuário>.github.io/<repositório>/`.

## Atualização das releases

A página consulta a [API de releases do GitHub](https://docs.github.com/en/rest/releases/releases) ao exibir uma build, incluindo as pré-releases dos forks que as publicam. O seletor atualiza a versão e os dois links juntos quando encontra um pacote correspondente em uma release mais nova.

As versões incluídas no site ficam em `site/releases.js` e funcionam como referência se o GitHub estiver indisponível ou se o arquivo ainda não tiver sido publicado. Se um projeto mudar o formato da tag, o nome da branch ou o nome dos arquivos, revise os padrões em `site/releases.js` e os nomes dos pacotes em `site/app.js`. Para atualizar a referência da tela inicial, ajuste também os links e a versão em `site/index.html`.

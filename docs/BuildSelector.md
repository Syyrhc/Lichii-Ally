# Seletor de builds Lichii

A página usa HTML, CSS e JavaScript sem dependências. Para testar localmente, abra `docs/index.html` no navegador.

## Publicação no GitHub Pages

1. Em **Settings → Pages → Build and deployment → Source**, escolha **Deploy from a branch**.
2. Em **Branch**, selecione **main** e **/docs** e clique em **Save**.
3. Depois da publicação, a tela **Settings → Pages** mostra o endereço do site.

O seletor usa caminhos relativos e funciona em endereços do tipo `https://<usuário>.github.io/<repositório>/`. Se a fonte estiver configurada como **GitHub Actions**, execute o workflow manual **Publicar seletor no GitHub Pages**, que também publica `docs/`.

## Atualização das releases

A página consulta a [API de releases do GitHub](https://docs.github.com/en/rest/releases/releases) ao exibir uma build, incluindo as pré-releases dos forks que as publicam. O seletor atualiza a versão e os dois links juntos quando encontra um pacote correspondente em uma release mais nova.

As versões incluídas no site ficam em `docs/releases.js` e funcionam como referência se o GitHub estiver indisponível ou se o arquivo ainda não tiver sido publicado. Se um projeto mudar o formato da tag, o nome da branch ou o nome dos arquivos, revise os padrões em `docs/releases.js` e os nomes dos pacotes em `docs/app.js`. Para atualizar a referência da tela inicial, ajuste também os links e a versão em `docs/index.html`.

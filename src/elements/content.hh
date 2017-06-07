<?hh

class :content extends :x:element {
  attribute string title;
  attribute Vector<string> components = Vector {};

  protected function render(): XHPRoot {
    $head = (
      <head>
        <title>{$this->:title}</title>
        <script src="/vendor/react.js"></script>
        <script src="/vendor/react-dom.js"></script>
        <script src="/vendor/xhpjs.js"></script>
        <script src="/main.js"></script>
        <link href="https://fonts.googleapis.com/css?family=Roboto|Roboto+Slab" rel="stylesheet" />
        <link rel="stylesheet" href="/main.css" />
      </head>
    );
    foreach ($this->:components as $component) {
      $head->appendChild(<script src={"/components/" . $component . ".js"}></script>);
    }
    return (
      <html>
      {$head}
      <body>
        {requestUri() === "/" ? "" :
          <header>
            <h3>Crossword App</h3>
            <nav>
              <a href="/">Home</a>
              <a href="/create">Create</a>
              <a href="/puzzles">Play</a>
            </nav>
          </header>
        }
        <main>
          {$this->getChildren()}
        </main>
      </body>
      </html>
    );
  }
}

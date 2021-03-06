<?hh
require_once("../src/elements/pages/base-page.hh");
require_once("../src/elements/content.hh");

class :index-page extends :base-page {
  protected function render(): XHPRoot {
    return (
      <content title="Cross Word App">
        <h1>Cross Word App</h1>
        <nav>
          <a href="create">Create</a>
          <a href="puzzles">Play</a>
        </nav>
      </content>
    );
  }
}

<?hh
require_once("../src/elements/BasePage.hh");
require_once("../src/elements/page.hh");

class :index-page extends :base-page {
  protected function render(): XHPRoot {
    return (
      <page title="Cross Word App">
        <h1>Cross Word App</h1>
        <nav>
          <ul>
            <li><a href="create">Create</a></li>
          </ul>
        </nav>
      </page>
    );
  }
}
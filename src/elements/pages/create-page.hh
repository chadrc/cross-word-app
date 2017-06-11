<?hh
require_once("../src/elements/pages/base-page.hh");
require_once("../src/elements/content.hh");
require_once("../src/elements/cross-word-form.hh");

class :create-page extends :base-page {
  protected function render(): XHPRoot {
    return (
      <content title="Create a Cross Word" components={Vector {"CrossWordForm"}}>
        <h1>Create Cross Word</h1>
        <x:js-scope>
          <cross-word-form />
        </x:js-scope>
      </content>
    );
  }
}

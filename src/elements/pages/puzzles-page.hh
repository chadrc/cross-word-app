<?hh
require_once("../src/elements/content.hh");
require_once("../src/elements/pages/base-page.hh");

class :puzzles-page extends :base-page {
  protected function render(): XHPRoot {
    return (
      <content title="Puzzles">
        <h1>Pick a puzzle</h1>
        <table>
          <tbody>
            <tr>
              <th>Word Count</th>
              <th>Size</th>
              <th>Created</th>
            </tr>
          </tbody>
        </table>
      </content>
    );
  }
}

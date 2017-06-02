<?hh
require_once("../shared/page.hh");

function create_route() {
  return (
  <page title="Create a Cross Word">
    <h1>Create Cross Word</h1>
    <form action="make" method="post">
      <label for="words">Words (Comma separated)</label>
      <textarea id="words" name="words" />

      <label for="hints">Hints (Comma separated)</label>
      <textarea id="hints" name="hints" />

      <button type="submit">Make</button>
    </form>
  </page>
  );
}

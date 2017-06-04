<?hh
require_once("../src/elements/BasePage.hh");
require_once("../src/elements/main.hh");

class :create-page extends :base-page {
  protected function render(): XHPRoot {
    return requestMethod() === "POST" ? $this->form_submit() : $this->create_form();
  }

  private function create_form(): XHPRoot {
    return (
      <main title="Create a Cross Word">
        <h1>Create Cross Word</h1>
        <form action="create" method="post">
          <label for="words">Words (Comma separated)</label>
          <textarea id="words" name="words" />

          <label for="hints">Hints (Comma separated)</label>
          <textarea id="hints" name="hints" />

          <button type="submit">Make</button>
        </form>
      </main>
    );
  }

  private function form_submit(): XHPRoot {
    $words = $this->post_data["words"];
    $hints = $this->post_data["hints"];

    $words_array = new Vector(explode(",", $words));
    $hints_array = new Vector(explode(",", $hints));

    if ($words_array->count() !== $hints_array->count()) {
      return (
        <main title="Couldn't Create Cross Word">
          <h3>Number of words and hints did not match.</h3>
          <a href="create">Try Again</a>
        </main>
      );
    } else {
      $word_hint_map = Map {};
      for ($i=0; $i<$words_array->count(); $i++) {
        $word = trim($words_array[$i]);
        $hint = trim($hints_array[$i]);
        $word_hint_map[$word] = $hint;
      }

      $cross_word = null;
      try {
        $cross_word = create_cross_word($word_hint_map);
      } catch (CrossWordGenerationException $e) {
        return (
          <main title="Couldn't Create Cross Word">
            <h3>Sorry, we were unable to generate a cross word with your words.</h3>
            <h5>You may either adjust your words or try to submit again.</h5>
          </main>
        );
      }

      $store = DB()->saveCrossWord($cross_word);
      header("Location: /puzzle/" . $store->get_id());
      return <main title="Crossword Created" />;
    }
  }
}

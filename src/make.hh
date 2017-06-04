<?hh
require_once("../src/pages/page.hh");

function make_route() {
  $words = postString("words");
  $hints = postString("hints");

  $words_array = new Vector(explode(",", $words));
  $hints_array = new Vector(explode(",", $hints));

  if ($words_array->count() !== $hints_array->count()) {
    echo (
      <page title="Couldn't Create Cross Word">
        <h3>Number of words and hints did not match.</h3>
        <a href="create">Try Again</a>
      </page>
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
      echo (
        <page title="Couldn't Create Cross Word">
          <h3>Sorry, we were unable to generate a cross word with your words.</h3>
          <h5>You may either adjust your words or try to submit again.</h5>
        </page>
      );
      return;
    }

    $store = saveCrossWord($cross_word);

    header("Location: /puzzle/" . $store->get_id());
  }
}

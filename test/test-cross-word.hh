<?hh
require_once("shared/cross-word.hh");

function test_basic() {
  get_cross_word(Map {
    "myWord" => "First Word",
    "myOtherWord" => "Another Word",
    "dogs" => "Furry animal.",
    "dragons" => "Mythical creature."
  });
}

function test_multi_join() {
  get_cross_word(Map {
    "oooowooo" => "First Word",
    "ooooxooo" => "Another Word",
    "oooohooo" => "Furry animal.",
    "ooookooo" => "Mythical creature."
  });
}

function get_cross_word(Map<string, string> $word_hint_map) {
  $cross_word = null;
  try {
    $cross_word = create_cross_word($word_hint_map);
  } catch (CrossWordCreationFailed $e) {
    foreach ($e->get_attempts() as $attempt) {
      echo $attempt;
    }
  }

  if ($cross_word !== null) {
    echo $cross_word->get_grid();
  }
}

// test_basic();
test_multi_join();

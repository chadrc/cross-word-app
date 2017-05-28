<?hh
require_once("shared/cross-word.hh");

$word_hint_map = Map {
  "myWord" => "First Word",
  "myOtherWord" => "Another Word",
  "dogs" => "Furry animal."
};

$cross_word = create_cross_word($word_hint_map);

print_grid($cross_word->get_grid());
foreach ($cross_word->get_grid() as $index => $col) {
  foreach ($col as $index => $cell) {
    $x = $cell->get_x();
    $y = $cell->get_y();
    $letter = $cell->get_letter();
    $join = "";
    if ($cell->get_join() !== null) {
      $join = " | (" . $cell->get_join()->get_x() . ", " . $cell->get_join()->get_y() . ") - " . $cell->get_join()->get_letter();
    }
    echo "(" . $x . ", " . $y . ") - " . $letter . $join . "\n";
  }
}

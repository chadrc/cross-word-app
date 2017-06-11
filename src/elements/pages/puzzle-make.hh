<?hh

function PuzzleMake() {
  $data = new Map((array) json_decode(fetchPOSTBody()));
  $wordPairs = new Vector((array)$data["words"]);

  $words_array = new Vector($wordPairs->map(($item) ==> {
    return $item->word;
  }));
  $hints_array = new Vector($wordPairs->map(($item) ==> {
    return $item->hint;
  }));

  if ($words_array->count() !== $hints_array->count()) {
    return json_encode(Map {
      "message" => "Number of words and hints did not match."
    });
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
      http_response_code(500);
      return json_encode(Map {
        message => "Sorry, we were unable to generate a cross word with your words. You may either adjust your words or try to submit again."
      });
    }

    $store = DB()->saveCrossWord($cross_word);

    return json_encode(Map {
      "message" => "Puzzle made",
      "puzzleId" => $store->get_id()
    });
  }
}

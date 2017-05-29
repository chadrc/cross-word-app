<?hh
require_once("shared/CrossWordGenerator.hh");
require_once("shared/utils.hh");

function create_cross_word(Map<string, string> $wordHintMap): CrossWord {
  $attempts = Vector {};
  $cross_word = null;
  while ($attempts->count() < 10) {
    try {
      $cross_word = make_cross_word($wordHintMap);
      break;
    } catch (CrossWordGenerationException $e) {
      $attempts[] = $e;
    }
  }
  if ($cross_word === null) {
    throw new CrossWordCreationFailed($attempts);
  }
  return $cross_word;
}

function make_cross_word(Map<string, string> $wordHintMap): CrossWord {
  $generator = new CrossWordGenerator($wordHintMap);
  return $generator->get_cross_word();
}

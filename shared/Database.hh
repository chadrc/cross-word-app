<?hh

function recordRequest(array $postParams, array $getParams, string $uri) {
  $client = new MongoDB\Client("mongodb://172.17.0.1:27017");
  $collection = $client->CrossWordApp->RequestLog;
  $collection->insertOne(['postParams' => fetchPOST(), 'getParams' => fetchGET(), 'uri' => requestUri(), 'created' => new MongoDB\BSON\UTCDateTime()]);
}

<?php

if ($_SERVER["REQUEST_URI"]!='/settings') 
{
    return false;
}

if($_SERVER['REQUEST_METHOD']=='POST')
{
    file_put_contents('settings.json', file_get_contents('php://input'));
    echo 'saved';
}
else
{
$settings = json_encode([
  'ssid' => 'foo',
  'password' => 'bar',
  'maxChannel' => 5,
  'levels' => [
    [
      'time'=>1439,
      'power'=>50,
      'c' => [ 2048, 2048, 2048, 2048, 2048]
    ]
  ]
]);

$x = file_get_contents('settings.json');
if($x) $settings = $x;

    echo $settings;
}
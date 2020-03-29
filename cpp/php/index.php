<?php

switch($_SERVER["REQUEST_URI"])
{
case '/':
    echo file_get_contents('index.htm');
    break;
case '/settings/save':
    file_put_contents('settings.json', file_get_contents('php://input'));
    echo 'saved';
    break;
case '/settings':
    $settings = '{}'; /*json_encode([
      'ssid' => 'foo',
      'password' => 'bar',
      'maxChannel' => 5,
      'levels' => [
        [
          'time'=>1439,
          'power'=>50,
          'c' => [ 2048, 2048, 2048, 2048, 2048]
        ],
        'override' => [
          'channels' => [
            'enabled' => false,
            'values' => [32, 64, 128, 256, 512]
          ]
        ]
      ]
    ]);     */
    $x = @file_get_contents('settings.json');
    if($x) $settings = $x;
    echo $settings;
    break;
case '/status':
    echo json_encode([
        'temperature' => 20,
        'channels' => [40, 150, 1200, 1024]
                     ]);
    break;
default:
    return false;
}
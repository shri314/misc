<?php
$url = 'https://accounts.google.com/o/oauth2/token';

$post_data = array(
                    'client_id'     =>   'FILL',
                    'client_secret' =>   'FILL',
                    'code'          =>   'FILL',
                    'redirect_uri'  =>   'FILL',
                    'grant_type'    =>   'authorization_code'
                  );

$ch = curl_init();

curl_setopt($ch, CURLOPT_URL, $url);
curl_setopt($ch, CURLOPT_POST, 1);
curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

$result = curl_exec($ch);

print_r( $result );

$token = json_decode($result);

echo $token->refresh_token . "\n";
?>

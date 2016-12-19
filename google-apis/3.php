<?php
    $token_url     = 'https://accounts.google.com/o/oauth2/token';

    $post_data = array(
                        'client_id'     =>   'FILL',
                        'client_secret' =>   'FILL',
                        'refresh_token' =>   'FILL',
                        'grant_type'    =>   'refresh_token'
                     );

    $ch = curl_init();

    curl_setopt($ch, CURLOPT_URL, $token_url);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    $result = curl_exec($ch);

    print_r( $result );
?>

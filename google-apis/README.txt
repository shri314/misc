0. Get a Client ID (skip if already done)

   - Go to http://console.developers.google.com
   - Create project if not already created
   - Enable Google Calendar API if not already enabled
   - Create OAuth 2.0 client IDs credential
   - Download the JSON formatted file containing:
      client_id, client_secret, redirect_uris

1. Get one-time AUTH code (skip if already done)

   - Fill client_id (from 0), redirect_uri (from 0) in 1.php
   - Fill scope (according to API you want to use) in 1.php [Ex: https://www.googleapis.com/auth/calendar]
   - Run php -q 1.php
   - Hit the URL from script via browser
   - Login to gmail account, accept authorization
   - Record the one-time AUTH code

2. Get the refresh token (skip if already have a valid temporary access token)

   - Fill client_id (from 0), client_secret (from 0), code (from 1), redirect_uri (from 0) in 2.php
   - Run php -q 2.php
   - Record the refresh_token returned

3. Get the temporary access token (valid for an hour)

   - Fill in client_id (same as 1), client_secret (from 0), refresh_token (from 2) in 3.php
   - Run php -q 3.php
   - Record the access_token returned

4. Access the API

   - Fill in the access_token (from 3) in 4.sh
   - Change API as per requirements (See https://developers.google.com/google-apps/calendar/v3/reference/)
   - Run 4.sh
   - If access token has expired, go to step 3.

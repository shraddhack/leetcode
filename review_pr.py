import os
import openai
import requests
from github import Github

# Set up the OpenAI API key from environment variables
openai.api_key = os.getenv("OPENAI_API_KEY")

# GitHub token to interact with the repository
GITHUB_TOKEN = os.getenv("GITHUB_TOKEN")

# Function to fetch changed files in the pull request
def fetch_pr_files(pr_url):
    response = requests.get(pr_url)
    files = response.json()
    changes = ""
    for file in files:
        if file['status'] == 'modified' or file['status'] == 'added':
            changes += f"\n\n--- {file['filename']} ---\n{file['patch']}\n"
    return changes

# Function to get PR data from GitHub
def get_pull_request_data():
    # GitHub API client
    g = Github(GITHUB_TOKEN)
    repo = g.get_repo("shraddhack/leetcode")  # Replace with your repo name
    #pr = repo.get_pull(int(os.getenv('PR_ID')))
    pr_id = os.getenv('PR_ID')
    if not pr_id:
        raise ValueError("PR_ID environment variable is not set.")
    pr_id = int(pr_id)  # Now you can safely convert it to an integer
    # Fetch the PR using the pr_id (your existing code)
    pr = repo.get_pull(pr_id)
    return pr

# Get GPT-4 review for the changes
#def get_gpt_review(changes):
 #   response = openai.ChatCompletion.create(
  #      model="gpt-4",  # Or use "gpt-3.5-turbo"
   #     messages=[
    #        {"role": "system", "content": "You are a helpful assistant for reviewing code."},
     #       {"role": "user", "content": f"Please review the following code changes:\n\n{changes}"},
      #  ],
       # temperature=0.5,
   # )

    # Extract the review from the response
    #review = response['choices'][0]['message']['content']
    #return review


def get_gpt_review(changes):
        # Make the API call
        response = openai.completions.create(
            model="gpt-3.5-turbo",  # Use a model available within your quota
            prompt=f"Here are the changes in the code:\n{changes}",
            max_tokens=150,
            temperature=0.5
        )

        return response.choices[0].text.strip()

# Function to add a comment to a PR
def post_comment(pr, review):
    pr.create_issue_comment(review)

def main():
    # Get PR data (we assume the PR ID is passed as an environment variable)
    pr = get_pull_request_data()
    pr_url = pr.url + "/files"
    
    # Fetch the changed files in the PR
    changes = fetch_pr_files(pr_url)
    
    # Generate review using GPT
    review = get_gpt_review(changes)
    
    # Post the review as a comment
    post_comment(pr, review)

if __name__ == "__main__":
    main()


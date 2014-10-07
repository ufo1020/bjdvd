#include "page.h"

PostDetails::PostDetails()
{

}

PostDetails::~PostDetails()
{

}

PostDetails::PostDetails(const QStringList& list)
{
    m_subject = list[0];
    m_date = list[1];
    m_author  = list[2];
    m_size = list[3];
    m_clicks = list[4];
    m_replies = list[5];
    m_body = list[6];
}


//! PageContect details
PageContent::PageContent()
{
}

PageContent::~PageContent()
{

}

void PageContent::add_post(const PostDetails& post)
{
    m_posts.push_back(post);
}

PostDetails PageContent::get_post(int32_t index)
{
    Q_ASSERT(index >=0 && index <m_posts.size());
    return std::move(m_posts[index]);
}

std::vector<PostDetails> PageContent::get_posts()
{
    return std::move(m_posts);
}


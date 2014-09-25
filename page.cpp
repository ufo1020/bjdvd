#include "page.h"

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

PostDetails& PageContent::get_post(int32_t index)
{
    Q_ASSERT(index >=0 && index <m_posts.size());
    return std::move(m_posts[index]);
}


#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity )
{
  available_capacity_ = capacity_;
}

bool Writer::is_closed() const
{
  return is_closed_;
}

void Writer::push( string data )
{
  if (is_closed_) {
    return;
  }
  size_t limit_size = available_capacity();
  auto len = min( data.size(), limit_size );
  if ( len < data.size() ) {
    data = data.substr( 0, len );
  }
  if ( len > 0 ) {
    available_capacity_ -= len;
    bytes_pushed_ += len;
    bytes_buffer_ += len;
    buffer_.emplace_back( move( data ) );
    buffer_view_.emplace_back( buffer_.back() );
  }
}

void Writer::close()
{
  is_closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  return available_capacity_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  return is_closed_ && bytes_buffer_ == 0;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_poped_;
}

string_view Reader::peek() const
{
  return buffer_view_.front();
}

void Reader::pop( uint64_t len )
{
  len = min( len, bytes_buffer_ );
  auto size = len;
  while ( len > 0 ) {
    auto bu_view_size = buffer_view_.front().size();
    if ( len >= bu_view_size ) {
      buffer_view_.pop_front();
    } else {
      buffer_view_.front().remove_prefix( len );
      break;
    }
    len -= bu_view_size;
  }

  bytes_buffer_ -= size;
  bytes_poped_ += size;
  available_capacity_ += size;
}

uint64_t Reader::bytes_buffered() const
{
  return bytes_buffer_;
}
